#include "ui/UIRoot.h"

#include "utils/IterationGuard.h"

namespace Rival {

UIRoot::UIRoot()
{
    setResizeMode(ResizeMode::Stretch, ResizeMode::Stretch);
}

void UIRoot::update(const glm::ivec2& viewportSize)
{
    removeQueuedDescendants();
    measure();
    arrange({ 0, 0 }, viewportSize);
}

bool UIRoot::sendMouseMoveEvent(const MouseMoveEvent& evt)
{
    {
        IterationGuard guard(eventDispatchDepth);

        UIElement* newHoveredElement = containsPoint(evt.position) ? dispatchMouseMoveEvent(evt) : nullptr;

        if (newHoveredElement != hoveredElement)
        {
            if (hoveredElement)
            {
                hoveredElement->onMouseExit();
            }

            hoveredElement = newHoveredElement;

            if (hoveredElement)
            {
                hoveredElement->onMouseEnter();
            }
        }
    }

    // If this event was sent from within another event handler, wait for the outermost event to finish
    if (eventDispatchDepth == 0)
    {
        removeQueuedDescendants();
    }

    return hoveredElement != nullptr;
}

bool UIRoot::sendMouseButtonEvent(const MouseButtonEvent& evt)
{
    bool isHandled = false;

    {
        IterationGuard guard(eventDispatchDepth);
        isHandled = containsPoint(evt.position) && dispatchMouseButtonEvent(evt);
    }

    // If this event was sent from within another event handler, wait for the outermost event to finish
    if (eventDispatchDepth == 0)
    {
        removeQueuedDescendants();
    }

    return isHandled;
}

bool UIRoot::isDispatchingEvent() const
{
    return eventDispatchDepth > 0;
}

UIElement* UIRoot::getHoveredElement() const
{
    return hoveredElement;
}

void UIRoot::onDescendantRemoved(const UIElement& descendant)
{
    // Don't keep a pointer to an element that's no longer in the tree
    if (hoveredElement && hoveredElement->isWithin(descendant))
    {
        hoveredElement = nullptr;
    }
}

}  // namespace Rival
