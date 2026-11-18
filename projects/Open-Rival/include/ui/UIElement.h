#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <optional>
#include <vector>

#include "input/Input.h"
#include "ui/UIVisual.h"

namespace Rival {

/** Enum that determines how a UIElement should determine its size along one axis. */
enum class ResizeMode
{
    /** The element should use its minimum size. */
    Fixed,

    /** The element should stretch to fill the space available to it (but never shrink below its minimum size).
     * Within a FlatContainer, stretching elements along the container's direction share any leftover space. */
    Stretch
};

/** A point within a rectangle that an element attaches to.
 * This determines where a Fixed element sits within the space available to it. */
enum class Anchor
{
    TopLeft,
    Top,
    TopRight,
    Left,
    Centre,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

/**
 * A part of the UI.
 *
 * UIElements form a hierarchical structure, so an instance of this class could be a single element (e.g. a button), or
 * an entire menu screen.
 *
 * ---
 *
 * Positioning:
 *
 *   Positions are measured from the top-left corner of the parent. Children that extend beyond the origin in the
 *   negative direction are excluded from bounds calculations, and cannot receive mouse events.
 *
 * Layout:
 *
 *   Layout happens in 2 passes, driven by UIRoot:
 *    1. Measure (bottom-up): each element calculates its minimum size, based on its own minSize and its children.
 *    2. Arrange (top-down): each element is given a slot by its parent, and sizes and positions itself within it
 *       according to its resize modes, anchor and offset. It then arranges its own children.
 *
 *   By default, each child's slot is the whole of its parent. Containers override this to provide their own slots.
 *
 * Input handling:
 *
 *   When an input event is sent to a UIElement, it gets passed down the heirarchy to the deepest child for which it is
 *   is relevant (for example, a mouse event will be passed to the deepest child whose bounds contain the cursor
 *   position). If the child does not handle the event itself, it bubbles back up, giving all elements in the chain the
 *   chance to handle it.
 */
class UIElement
{
public:
    virtual ~UIElement() = default;

    /** Gets an ID that is unique to this element for the lifetime of the program. */
    int getId() const;

    /** Adds a child to this element. */
    void addChild(std::unique_ptr<UIElement> child);

    /** Removes a child from this element, if present.
     * Returns the unique_ptr that was removed, or nullptr.
     * If this is called while an event is being dispatched, the child is queued for removal instead (see
     * queueRemoval()), and nullptr is returned. */
    std::unique_ptr<UIElement> removeChild(const UIElement* childToRemove);

    /** Marks this element to be removed from its parent (and destroyed) once the current event has been handled,
     * or during the next UIRoot update. */
    void queueRemoval();

    UIElement* getParent() const;

    /** Returns a read-only list of children. */
    const std::vector<std::unique_ptr<UIElement>>& getAllChildren() const;

    void setResizeMode(ResizeMode horizontal, ResizeMode vertical);
    ResizeMode getHorizontalResizeMode() const;
    ResizeMode getVerticalResizeMode() const;

    void setAnchor(Anchor newAnchor);

    /** Sets the offset of this element from its anchor point. */
    void setOffset(const glm::ivec2& newOffset);

    void setMinSize(const glm::ivec2& newMinSize);

    /** Gets the minimum size of this element, taking into account its children, as of the last layout. */
    glm::ivec2 getRequiredSize() const;

    /** Gets the position of this element relative to its parent, as of the last layout. */
    glm::ivec2 getPosition() const;

    /** Gets the size of this element, as of the last layout. */
    glm::ivec2 getSize() const;

    /** Determines whether this element is the given element, or one of its descendants. */
    bool isWithin(const UIElement& ancestor) const;

    /** Determines whether this element contains a point (relative to this element). */
    bool containsPoint(const glm::ivec2& point) const;

    /** Gets the visual that should be drawn for this element, or nullptr if it has none. */
    const UIVisual* getVisual() const;

    /** Called when this element becomes the hovered element. */
    virtual void onMouseEnter() {}

    /** Called when this element stops being the hovered element. */
    virtual void onMouseExit() {}

    /** Receives a mouse move event dispatched to this UIElement. The position is relative to this element.
     * Returns true if this element should become the hovered element, or false if the event should bubble up. */
    virtual bool onMouseMoveEvent(const MouseMoveEvent& evt);

    /** Receives a mouse button event dispatched to this UIElement. The position is relative to this element.
     * Returns true if the event was handled, or false if it should bubble up. */
    virtual bool onMouseButtonEvent(const MouseButtonEvent& evt);

protected:
    /** Recalculates the required size of all children, and then this element itself. */
    void measure();

    /** Sizes and positions this element within the given slot (relative to the parent), then arranges its children.
     * Assumes that `recalculateRequiredSize` has already been called. */
    void arrange(const glm::ivec2& slotPosition, const glm::ivec2& slotSize);

    /** Gets the minimum size required to fit this element's visual and all of its children, based on their required
     * size. */
    virtual glm::ivec2 getContentMinSize() const;

    /** Arranges all children within this element. */
    virtual void arrangeChildren();

    /** Arranges a single child within the given slot (relative to this element). */
    static void arrangeChild(UIElement& child, const glm::ivec2& slotPosition, const glm::ivec2& slotSize);

    /** Called just before a descendant is removed from the tree.
     * By default, this passes the call up to the parent. */
    virtual void onDescendantRemoved(const UIElement& descendant);

    /** Determines whether an event is currently being dispatched anywhere in this element's tree.
     * By default, this asks the parent. */
    virtual bool isDispatchingEvent() const;

    /** Dispatches a mouse move event (relative to this element) to this element and its children.
     * Returns the element that accepted the event, or nullptr. */
    UIElement* dispatchMouseMoveEvent(const MouseMoveEvent& evt);

    /** Dispatches a mouse button event (relative to this element) to this element and its children.
     * Returns true if the event was handled. */
    bool dispatchMouseButtonEvent(const MouseButtonEvent& evt);

    /** Removes and destroys any descendants that have been queued for removal. */
    void removeQueuedDescendants();

    void setVisual(const UIVisual& newVisual);
    void clearVisual();

private:
    static inline int nextId = 0;

    int id = nextId++;

    UIElement* parent = nullptr;
    std::vector<std::unique_ptr<UIElement>> children;

    ResizeMode horizontalResizeMode = ResizeMode::Fixed;
    ResizeMode verticalResizeMode = ResizeMode::Fixed;
    Anchor anchor = Anchor::TopLeft;
    glm::ivec2 offset { 0, 0 };
    glm::ivec2 minSize { 0, 0 };

    bool isQueuedForRemoval = false;

    std::optional<UIVisual> visual;

    // Calculated during layout
    glm::ivec2 requiredSize { 0, 0 };
    glm::ivec2 position { 0, 0 };
    glm::ivec2 size { 0, 0 };
};

}  // namespace Rival
