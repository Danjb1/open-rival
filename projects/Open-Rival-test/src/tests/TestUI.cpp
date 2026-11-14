#include <catch2/catch_test_macros.hpp>

#include <glm/vec2.hpp>

#include <memory>

#include "input/Input.h"
#include "ui/UIElement.h"
#include "ui/UIRoot.h"
#include "ui/containers/FlatContainer.h"

using namespace Rival;

namespace TestUI {

/** UIElement that accepts mouse events and records what it received. */
class TestElement : public UIElement
{
public:
    bool onMouseMoveEvent(const MouseMoveEvent& evt) override
    {
        lastMousePosition = evt.position;
        return true;
    }

    bool onMouseButtonEvent(const MouseButtonEvent& evt) override
    {
        lastMousePosition = evt.position;
        ++numClicks;
        if (removeSelfWhenClicked)
        {
            queueRemoval();
        }
        if (outWasRemovedViaParent)
        {
            // Destroying this element now would be unsafe, since it is still handling the event
            *outWasRemovedViaParent = getParent()->removeChild(this) != nullptr;
        }
        return true;
    }

    void onMouseEnter() override
    {
        ++numEnters;
    }

    void onMouseExit() override
    {
        ++numExits;
    }

    glm::ivec2 lastMousePosition { -1, -1 };
    int numClicks = 0;
    int numEnters = 0;
    int numExits = 0;
    bool removeSelfWhenClicked = false;

    /** If set, this element asks its parent to remove it when clicked, and records whether that happened immediately.
     * This lives outside the element, since the element may have been destroyed by the time it is checked. */
    bool* outWasRemovedViaParent = nullptr;
};

/** Creates a fixed-size TestElement, and returns a pointer to it before handing over ownership. */
std::unique_ptr<TestElement> makeTestElement(const glm::ivec2& minSize, TestElement*& outPtr)
{
    auto elem = std::make_unique<TestElement>();
    elem->setMinSize(minSize);
    outPtr = elem.get();
    return elem;
}

const glm::ivec2 viewportSize { 800, 600 };

SCENARIO("Elements are positioned according to their anchor and offset", "[ui]")
{
    GIVEN("A root containing a fixed-size element")
    {
        UIRoot root;
        TestElement* elem = nullptr;
        root.addChild(makeTestElement({ 100, 50 }, elem));

        WHEN("the element uses the default anchor and an offset")
        {
            elem->setOffset({ 10, 20 });
            root.update(viewportSize);

            THEN("it is offset from the top-left corner")
            {
                REQUIRE(elem->getPosition() == glm::ivec2(10, 20));
                REQUIRE(elem->getSize() == glm::ivec2(100, 50));
            }
        }

        WHEN("the element is anchored to the bottom-right corner")
        {
            elem->setAnchor(Anchor::BottomRight);
            elem->setOffset({ -10, -20 });
            root.update(viewportSize);

            THEN("it is offset from the bottom-right corner")
            {
                REQUIRE(elem->getPosition() == glm::ivec2(800 - 100 - 10, 600 - 50 - 20));
            }
        }

        WHEN("the element is anchored to the centre")
        {
            elem->setAnchor(Anchor::Centre);
            root.update(viewportSize);

            THEN("it is centred")
            {
                REQUIRE(elem->getPosition() == glm::ivec2(350, 275));
            }
        }

        WHEN("the element stretches horizontally")
        {
            elem->setResizeMode(ResizeMode::Stretch, ResizeMode::Fixed);
            root.update(viewportSize);

            THEN("it fills the width of its parent, but keeps its height")
            {
                REQUIRE(elem->getSize() == glm::ivec2(800, 50));
            }
        }
    }
}

SCENARIO("Elements grow to fit their children", "[ui]")
{
    GIVEN("A fixed-size panel containing an offset child")
    {
        UIRoot root;

        auto panel = std::make_unique<UIElement>();
        UIElement* panelPtr = panel.get();
        TestElement* child = nullptr;
        panel->addChild(makeTestElement({ 100, 50 }, child));
        root.addChild(std::move(panel));

        WHEN("the child has a positive offset")
        {
            child->setOffset({ 10, 20 });
            root.update(viewportSize);

            THEN("the panel grows to fit the child")
            {
                REQUIRE(panelPtr->getSize() == glm::ivec2(110, 70));
            }
        }

        WHEN("the child has a negative offset")
        {
            child->setOffset({ -10, -20 });
            root.update(viewportSize);

            THEN("the panel only grows to fit the part of the child that lies inside it")
            {
                REQUIRE(panelPtr->getSize() == glm::ivec2(90, 30));
            }
        }

        WHEN("the panel has a larger minimum size")
        {
            panelPtr->setMinSize({ 200, 200 });
            root.update(viewportSize);

            THEN("the panel uses its minimum size")
            {
                REQUIRE(panelPtr->getSize() == glm::ivec2(200, 200));
            }
        }
    }
}

SCENARIO("FlatContainer arranges its children in a row", "[ui]")
{
    GIVEN("A horizontal container with spacing, containing several children")
    {
        UIRoot root;

        auto container = std::make_unique<FlatContainer>(FlatDirection::Horizontal, 5);
        FlatContainer* containerPtr = container.get();
        TestElement* a = nullptr;
        TestElement* b = nullptr;
        TestElement* c = nullptr;
        container->addChild(makeTestElement({ 10, 20 }, a));
        container->addChild(makeTestElement({ 30, 40 }, b));
        container->addChild(makeTestElement({ 50, 10 }, c));
        root.addChild(std::move(container));

        WHEN("the container is laid out at its minimum size")
        {
            root.update(viewportSize);

            THEN("the container is just big enough to fit its children")
            {
                REQUIRE(containerPtr->getSize() == glm::ivec2(10 + 5 + 30 + 5 + 50, 40));
            }

            THEN("the children are placed side by side")
            {
                REQUIRE(a->getPosition() == glm::ivec2(0, 0));
                REQUIRE(b->getPosition() == glm::ivec2(15, 0));
                REQUIRE(c->getPosition() == glm::ivec2(50, 0));
            }
        }

        WHEN("the container stretches, and some children stretch along its direction")
        {
            containerPtr->setResizeMode(ResizeMode::Stretch, ResizeMode::Fixed);
            a->setResizeMode(ResizeMode::Stretch, ResizeMode::Fixed);
            c->setResizeMode(ResizeMode::Stretch, ResizeMode::Fixed);
            root.update(viewportSize);

            THEN("the stretching children share the leftover space")
            {
                // Leftover space is 800 - 100 = 700, which divides evenly between 2 children
                REQUIRE(a->getSize() == glm::ivec2(10 + 350, 20));
                REQUIRE(b->getSize() == glm::ivec2(30, 40));
                REQUIRE(c->getSize() == glm::ivec2(50 + 350, 10));
                REQUIRE(c->getPosition().x + c->getSize().x == 800);
            }
        }

        WHEN("a child stretches across the container's direction")
        {
            a->setResizeMode(ResizeMode::Fixed, ResizeMode::Stretch);
            root.update(viewportSize);

            THEN("it fills the height of the container")
            {
                REQUIRE(a->getSize() == glm::ivec2(10, 40));
            }
        }

        WHEN("a fixed child is anchored to the bottom")
        {
            c->setAnchor(Anchor::Bottom);
            root.update(viewportSize);

            THEN("it sits at the bottom of the container")
            {
                REQUIRE(c->getPosition() == glm::ivec2(50, 30));
            }
        }
    }
}

SCENARIO("Mouse events are sent to the topmost element under the cursor", "[ui]")
{
    GIVEN("A root containing a child, with an overlapping element on top")
    {
        UIRoot root;
        TestElement* bottom = nullptr;
        TestElement* top = nullptr;
        root.addChild(makeTestElement({ 100, 100 }, bottom));
        root.addChild(makeTestElement({ 50, 50 }, top));
        top->setOffset({ 20, 20 });
        root.update(viewportSize);

        WHEN("clicking on the overlapping region")
        {
            bool isHandled = root.sendMouseButtonEvent({ MouseButton::Left, { 30, 40 }, true });

            THEN("only the top element receives the event, relative to its own position")
            {
                REQUIRE(isHandled);
                REQUIRE(top->numClicks == 1);
                REQUIRE(top->lastMousePosition == glm::ivec2(10, 20));
                REQUIRE(bottom->numClicks == 0);
            }
        }

        WHEN("clicking outside both elements")
        {
            bool isHandled = root.sendMouseButtonEvent({ MouseButton::Left, { 500, 500 }, true });

            THEN("the event is not handled")
            {
                REQUIRE_FALSE(isHandled);
            }
        }
    }
}

SCENARIO("UIRoot tracks the hovered element", "[ui]")
{
    GIVEN("A root containing 2 elements side by side")
    {
        UIRoot root;
        TestElement* left = nullptr;
        TestElement* right = nullptr;
        root.addChild(makeTestElement({ 100, 100 }, left));
        root.addChild(makeTestElement({ 100, 100 }, right));
        right->setOffset({ 100, 0 });
        root.update(viewportSize);

        WHEN("the mouse moves onto the left element")
        {
            root.sendMouseMoveEvent({ { 50, 50 } });

            THEN("the left element is hovered")
            {
                REQUIRE(root.getHoveredElement() == left);
                REQUIRE(left->numEnters == 1);
            }

            AND_WHEN("the mouse moves within the left element")
            {
                root.sendMouseMoveEvent({ { 60, 50 } });

                THEN("the mouse does not enter it again")
                {
                    REQUIRE(left->numEnters == 1);
                    REQUIRE(left->numExits == 0);
                }
            }

            AND_WHEN("the mouse moves onto the right element")
            {
                root.sendMouseMoveEvent({ { 150, 50 } });

                THEN("the hovered element changes")
                {
                    REQUIRE(left->numExits == 1);
                    REQUIRE(right->numEnters == 1);
                    REQUIRE(root.getHoveredElement() == right);
                }
            }

            AND_WHEN("the mouse moves off both elements")
            {
                root.sendMouseMoveEvent({ { 500, 500 } });

                THEN("nothing is hovered")
                {
                    REQUIRE(left->numExits == 1);
                    REQUIRE(root.getHoveredElement() == nullptr);
                }
            }
        }
    }
}

SCENARIO("Elements can be removed during event handling", "[ui]")
{
    GIVEN("A hovered element that removes itself when clicked")
    {
        UIRoot root;
        TestElement* elem = nullptr;
        root.addChild(makeTestElement({ 100, 100 }, elem));
        elem->removeSelfWhenClicked = true;
        root.update(viewportSize);
        root.sendMouseMoveEvent({ { 50, 50 } });

        WHEN("the element is clicked")
        {
            root.sendMouseButtonEvent({ MouseButton::Left, { 50, 50 }, true });

            THEN("it is removed once the event has been handled")
            {
                REQUIRE(root.getAllChildren().empty());
                REQUIRE(root.getHoveredElement() == nullptr);
            }
        }
    }

    GIVEN("An element that asks its parent to remove it when clicked")
    {
        UIRoot root;
        auto panel = std::make_unique<UIElement>();
        UIElement* panelPtr = panel.get();
        TestElement* elem = nullptr;
        panel->addChild(makeTestElement({ 100, 100 }, elem));
        root.addChild(std::move(panel));
        bool wasRemovedImmediately = true;
        elem->outWasRemovedViaParent = &wasRemovedImmediately;
        root.update(viewportSize);

        WHEN("the element is clicked")
        {
            root.sendMouseButtonEvent({ MouseButton::Left, { 50, 50 }, true });

            THEN("the removal is deferred until the event has been handled")
            {
                REQUIRE_FALSE(wasRemovedImmediately);
                REQUIRE(panelPtr->getAllChildren().empty());
            }
        }
    }

    GIVEN("A hovered element inside a panel")
    {
        UIRoot root;
        auto panel = std::make_unique<UIElement>();
        UIElement* panelPtr = panel.get();
        TestElement* elem = nullptr;
        panel->addChild(makeTestElement({ 100, 100 }, elem));
        root.addChild(std::move(panel));
        root.update(viewportSize);
        root.sendMouseMoveEvent({ { 50, 50 } });

        WHEN("the panel is removed immediately")
        {
            std::unique_ptr<UIElement> removed = root.removeChild(panelPtr);

            THEN("the root no longer refers to the hovered element")
            {
                REQUIRE(removed.get() == panelPtr);
                REQUIRE(removed->getParent() == nullptr);
                REQUIRE(root.getHoveredElement() == nullptr);
            }
        }
    }
}

}  // namespace TestUI
