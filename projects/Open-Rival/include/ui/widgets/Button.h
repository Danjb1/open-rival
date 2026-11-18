#pragma once

#include <functional>

#include "ui/UIElement.h"

namespace Rival {

/** Button element that emits an event when pressed. By default, it is the size of its normal visual. */
class Button : public UIElement
{
public:
    Button(const UIVisual& newNormalVisual, const UIVisual& newPressedVisual);

    /** Sets the function to call when this button is clicked. */
    void setOnClick(std::function<void()> callback);

    void onMouseExit() override;
    bool onMouseMoveEvent(const MouseMoveEvent& evt) override;
    bool onMouseButtonEvent(const MouseButtonEvent& evt) override;

private:
    UIVisual normalVisual;
    UIVisual pressedVisual;
    std::function<void()> onClick;
};

}  // namespace Rival
