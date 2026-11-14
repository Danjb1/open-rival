#pragma once

#include <glm/vec2.hpp>

#include "input/Input.h"
#include "ui/UIElement.h"

namespace Rival {

/**
 * Top-level UIElement that fills the viewport.
 *
 * This is the entry point for layout and input, and keeps track of the hovered element.
 */
class UIRoot : public UIElement
{
public:
    UIRoot();

    /** Removes any queued elements, then lays out the whole tree to fill the given viewport. */
    void update(const glm::ivec2& viewportSize);

    /** Sends a mouse move event (relative to the viewport) to the UI, updating the hovered element.
     * Returns true if some element is now hovered. */
    bool sendMouseMoveEvent(const MouseMoveEvent& evt);

    /** Sends a mouse button event (relative to the viewport) to the UI.
     * Returns true if the event was handled. */
    bool sendMouseButtonEvent(const MouseButtonEvent& evt);

    UIElement* getHoveredElement() const;

protected:
    void onDescendantRemoved(const UIElement& descendant) override;
    bool isDispatchingEvent() const override;

private:
    UIElement* hoveredElement = nullptr;

    /** Number of events currently being dispatched (events may be sent from within event handlers). */
    int eventDispatchDepth = 0;
};

}  // namespace Rival
