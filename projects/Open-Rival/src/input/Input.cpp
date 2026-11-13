#include "input/Input.h"

namespace Rival {

KeyEvent::KeyEvent(KeyCode key)
    : key(key)
{
}

MouseButtonEvent::MouseButtonEvent(MouseButton button, int x, int y)
    : button(button)
    , x(x)
    , y(y)
{
}

MouseMoveEvent::MouseMoveEvent(int x, int y)
    : x(x)
    , y(y)
{
}

MouseWheelEvent::MouseWheelEvent(int delta)
    : delta(delta)
{
}

void Input::onKeyDown(const KeyEvent& evt)
{
    if (evt.key != KeyCode::Unknown)
    {
        pressedKeys.insert(evt.key);
    }
}

void Input::onKeyUp(const KeyEvent& evt)
{
    pressedKeys.erase(evt.key);
}

void Input::onMouseDown(const MouseButtonEvent& evt)
{
    if (evt.button != MouseButton::Other)
    {
        pressedMouseButtons.insert(evt.button);
    }
    mouseX = evt.x;
    mouseY = evt.y;
}

void Input::onMouseUp(const MouseButtonEvent& evt)
{
    pressedMouseButtons.erase(evt.button);
    mouseX = evt.x;
    mouseY = evt.y;
}

void Input::onMouseMoved(const MouseMoveEvent& evt)
{
    mouseX = evt.x;
    mouseY = evt.y;
}

void Input::releaseAll()
{
    pressedKeys.clear();
    pressedMouseButtons.clear();
}

bool Input::isKeyPressed(KeyCode key) const
{
    return pressedKeys.find(key) != pressedKeys.cend();
}

bool Input::isMouseButtonPressed(MouseButton button) const
{
    return pressedMouseButtons.find(button) != pressedMouseButtons.cend();
}

}  // namespace Rival
