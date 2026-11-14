#include "input/Input.h"

namespace Rival {

KeyEvent::KeyEvent(KeyCode key, bool isPress)
    : key(key)
    , isPress(isPress)
{
}

MouseButtonEvent::MouseButtonEvent(MouseButton button, glm::ivec2 position, bool isPress)
    : button(button)
    , position(position)
    , isPress(isPress)
{
}

MouseMoveEvent::MouseMoveEvent(glm::ivec2 position)
    : position(position)
{
}

MouseWheelEvent::MouseWheelEvent(int delta)
    : delta(delta)
{
}

void Input::onKeyEvent(const KeyEvent& evt)
{
    if (evt.key == KeyCode::Unknown)
    {
        return;
    }

    if (evt.isPress)
    {
        pressedKeys.insert(evt.key);
    }
    else
    {
        pressedKeys.erase(evt.key);
    }
}

void Input::onMouseButtonEvent(const MouseButtonEvent& evt)
{
    if (evt.button != MouseButton::Other)
    {
        if (evt.isPress)
        {
            pressedMouseButtons.insert(evt.button);
        }
        else
        {
            pressedMouseButtons.erase(evt.button);
        }
    }
    mousePos = evt.position;
}

void Input::onMouseMoveEvent(const MouseMoveEvent& evt)
{
    mousePos = evt.position;
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
