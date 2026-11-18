#include "ui/widgets/Button.h"

namespace Rival {

Button::Button(const UIVisual& newNormalVisual, const UIVisual& newPressedVisual)
    : normalVisual(newNormalVisual)
    , pressedVisual(newPressedVisual)
{
    setVisual(normalVisual);
}

void Button::setOnClick(std::function<void()> callback)
{
    onClick = std::move(callback);
}

void Button::onMouseExit()
{
    // Moving off the button cancels the press
    setVisual(normalVisual);
}

bool Button::onMouseMoveEvent(const MouseMoveEvent&)
{
    return true;
}

bool Button::onMouseButtonEvent(const MouseButtonEvent& evt)
{
    if (evt.button != MouseButton::Left)
    {
        return false;
    }

    if (evt.isPress)
    {
        setVisual(pressedVisual);
        return true;
    }

    setVisual(normalVisual);

    // TODO: Only click if the press also started on this button
    if (onClick)
    {
        onClick();
    }

    return true;
}

}  // namespace Rival
