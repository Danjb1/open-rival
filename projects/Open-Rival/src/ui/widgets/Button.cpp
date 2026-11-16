#include "ui/widgets/Button.h"

namespace Rival {

namespace {

// TMP: plain boxes until we have some proper button images
const BoxVisual normalVisual { { 0.4f, 0.4f, 0.4f, 1.f } };
const BoxVisual hoveredVisual { { 0.6f, 0.6f, 0.6f, 1.f } };

}  // namespace

Button::Button()
{
    setVisual(normalVisual);
}

void Button::setOnClick(std::function<void()> callback)
{
    onClick = std::move(callback);
}

void Button::onMouseEnter()
{
    setVisual(hoveredVisual);
}

void Button::onMouseExit()
{
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

    // TODO: Only click if the press also started on this button
    if (!evt.isPress && onClick)
    {
        onClick();
    }

    return true;
}

}  // namespace Rival
