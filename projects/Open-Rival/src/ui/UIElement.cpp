#include "ui/UIElement.h"

#include <glm/common.hpp>  // glm::max

#include <algorithm>  // std::find_if, std::max
#include <cstdlib>    // std::abs

#include "utils/LogUtils.h"

namespace Rival {

namespace {

/** Where an element sits along a single axis. */
enum class AxisAnchor
{
    Start,
    Middle,
    End
};

AxisAnchor getHorizontalAnchor(Anchor anchor)
{
    switch (anchor)
    {
    case Anchor::TopLeft:
    case Anchor::Left:
    case Anchor::BottomLeft:
        return AxisAnchor::Start;
    case Anchor::Top:
    case Anchor::Centre:
    case Anchor::Bottom:
        return AxisAnchor::Middle;
    default:
        return AxisAnchor::End;
    }
}

AxisAnchor getVerticalAnchor(Anchor anchor)
{
    switch (anchor)
    {
    case Anchor::TopLeft:
    case Anchor::Top:
    case Anchor::TopRight:
        return AxisAnchor::Start;
    case Anchor::Left:
    case Anchor::Centre:
    case Anchor::Right:
        return AxisAnchor::Middle;
    default:
        return AxisAnchor::End;
    }
}

/** Gets the the minimum size needed in one axis in order to a fit a child.
 * Since the parent can only grow in the positive axis (right/down), any part of the child that is pushed beyond the
 * parent's origin in the negative axis is ignored. */
int getRequiredSizeToFitChild(AxisAnchor axisAnchor, int childOffset, int childSize)
{
    switch (axisAnchor)
    {
    case AxisAnchor::Start:
        return std::max(0, childOffset + childSize);
    case AxisAnchor::Middle:
        return childSize + 2 * std::abs(childOffset);
    default:
        return std::max(0, childSize - childOffset);
    }
}

/** Position and size of an element, in a single axis. */
struct AxisLayout
{
    int position;
    int size;
};

/** Sizes and positions an element along one axis, within a slot. */
AxisLayout layoutAxis(
        ResizeMode resizeMode, AxisAnchor axisAnchor, int offset, int minSize, int slotPosition, int slotSize)
{
    int newSize = resizeMode == ResizeMode::Stretch ? std::max(minSize, slotSize) : minSize;

    int spareSpace = slotSize - newSize;
    int anchorOffset = 0;
    if (axisAnchor == AxisAnchor::Middle)
    {
        anchorOffset = spareSpace / 2;
    }
    else if (axisAnchor == AxisAnchor::End)
    {
        anchorOffset = spareSpace;
    }

    return { slotPosition + anchorOffset + offset, newSize };
}

}  // anonymous namespace

void UIElement::addChild(std::unique_ptr<UIElement> child)
{
    if (child->parent)
    {
        LOG_WARN("Tried to add a child to UIElement that already has a parent");
        return;
    }

    child->parent = this;
    children.push_back(std::move(child));
}

std::unique_ptr<UIElement> UIElement::removeChild(const UIElement* childToRemove)
{
    auto it = std::find_if(
            children.begin(), children.end(), [&](const auto& child) { return child.get() == childToRemove; });

    if (it == children.end())
    {
        // Child not found
        return nullptr;
    }

    std::unique_ptr<UIElement>& child = *it;

    if (isDispatchingEvent())
    {
        // Removing the child now could invalidate iterators or destroy an element that is still handling the event
        LOG_ERROR("Tried to remove a UIElement during event dispatch; use queueRemoval instead");
        child->queueRemoval();
        return nullptr;
    }

    onDescendantRemoved(*child);

    // Move the child out of the list, then erase the empty slot left behind
    std::unique_ptr<UIElement> removed = std::move(child);
    children.erase(it);

    removed->parent = nullptr;

    return removed;
}

void UIElement::queueRemoval()
{
    isQueuedForRemoval = true;
}

int UIElement::getId() const
{
    return id;
}

UIElement* UIElement::getParent() const
{
    return parent;
}

const std::vector<std::unique_ptr<UIElement>>& UIElement::getAllChildren() const
{
    return children;
}

void UIElement::setResizeMode(ResizeMode horizontal, ResizeMode vertical)
{
    horizontalResizeMode = horizontal;
    verticalResizeMode = vertical;
}

ResizeMode UIElement::getHorizontalResizeMode() const
{
    return horizontalResizeMode;
}

ResizeMode UIElement::getVerticalResizeMode() const
{
    return verticalResizeMode;
}

void UIElement::setAnchor(Anchor newAnchor)
{
    anchor = newAnchor;
}

void UIElement::setOffset(const glm::ivec2& newOffset)
{
    offset = newOffset;
}

void UIElement::setMinSize(const glm::ivec2& newMinSize)
{
    minSize = newMinSize;
}

glm::ivec2 UIElement::getRequiredSize() const
{
    return requiredSize;
}

glm::ivec2 UIElement::getPosition() const
{
    return position;
}

glm::ivec2 UIElement::getSize() const
{
    return size;
}

bool UIElement::containsPoint(const glm::ivec2& point) const
{
    return 0 <= point.x && point.x < size.x  //
            && 0 <= point.y && point.y < size.y;
}

bool UIElement::onMouseMoveEvent(const MouseMoveEvent&)
{
    return false;
}

bool UIElement::onMouseButtonEvent(const MouseButtonEvent&)
{
    return false;
}

void UIElement::measure()
{
    for (auto& child : children)
    {
        child->measure();
    }

    requiredSize = glm::max(minSize, getContentMinSize());
}

void UIElement::arrange(const glm::ivec2& slotPosition, const glm::ivec2& slotSize)
{
    AxisLayout x = layoutAxis(
            horizontalResizeMode, getHorizontalAnchor(anchor), offset.x, requiredSize.x, slotPosition.x, slotSize.x);
    AxisLayout y = layoutAxis(
            verticalResizeMode, getVerticalAnchor(anchor), offset.y, requiredSize.y, slotPosition.y, slotSize.y);

    position = { x.position, y.position };
    size = { x.size, y.size };

    arrangeChildren();
}

glm::ivec2 UIElement::getContentMinSize() const
{
    // By default, an element is big enough to fit its visual, and each child at its anchor point
    glm::ivec2 contentMinSize = visual ? getNaturalSize(*visual) : glm::ivec2 { 0, 0 };

    for (const auto& child : children)
    {
        int requiredWidth =
                getRequiredSizeToFitChild(getHorizontalAnchor(child->anchor), child->offset.x, child->requiredSize.x);
        int requiredHeight =
                getRequiredSizeToFitChild(getVerticalAnchor(child->anchor), child->offset.y, child->requiredSize.y);
        contentMinSize.x = std::max(contentMinSize.x, requiredWidth);
        contentMinSize.y = std::max(contentMinSize.y, requiredHeight);
    }

    return contentMinSize;
}

void UIElement::arrangeChildren()
{
    for (auto& child : children)
    {
        arrangeChild(*child, { 0, 0 }, size);
    }
}

void UIElement::arrangeChild(UIElement& child, const glm::ivec2& slotPosition, const glm::ivec2& slotSize)
{
    child.arrange(slotPosition, slotSize);
}

void UIElement::onDescendantRemoved(const UIElement& descendant)
{
    if (parent)
    {
        // Bubble up until we reach the UIRoot
        parent->onDescendantRemoved(descendant);
    }
}

bool UIElement::isDispatchingEvent() const
{
    return parent && parent->isDispatchingEvent();
}

const UIVisual* UIElement::getVisual() const
{
    return visual ? &*visual : nullptr;
}

void UIElement::setVisual(const UIVisual& newVisual)
{
    visual = newVisual;
}

void UIElement::clearVisual()
{
    visual.reset();
}

bool UIElement::isWithin(const UIElement& ancestor) const
{
    for (const UIElement* elem = this; elem; elem = elem->parent)
    {
        if (elem == &ancestor)
        {
            return true;
        }
    }
    return false;
}

UIElement* UIElement::dispatchMouseMoveEvent(const MouseMoveEvent& evt)
{
    // Iterate backwards over children, because those added last are on top
    for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
    {
        UIElement& child = *children[i];
        MouseMoveEvent childEvt { evt.position - child.position };

        if (!child.containsPoint(childEvt.position))
        {
            continue;
        }

        if (UIElement* acceptedBy = child.dispatchMouseMoveEvent(childEvt))
        {
            return acceptedBy;
        }
    }

    return onMouseMoveEvent(evt) ? this : nullptr;
}

bool UIElement::dispatchMouseButtonEvent(const MouseButtonEvent& evt)
{
    // Iterate backwards over children, because those added last are on top
    for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
    {
        UIElement& child = *children[i];
        MouseButtonEvent childEvt { evt.button, evt.position - child.position, evt.isPress };

        if (!child.containsPoint(childEvt.position))
        {
            continue;
        }

        if (child.dispatchMouseButtonEvent(childEvt))
        {
            return true;
        }
    }

    return onMouseButtonEvent(evt);
}

void UIElement::removeQueuedDescendants()
{
    // Copy the pointers first, since we can't erase from the list while iterating over it
    std::vector<const UIElement*> childrenToRemove;

    for (auto& child : children)
    {
        if (child->isQueuedForRemoval)
        {
            childrenToRemove.push_back(child.get());
        }
        else
        {
            child->removeQueuedDescendants();
        }
    }

    for (const UIElement* child : childrenToRemove)
    {
        removeChild(child);
    }
}

}  // namespace Rival
