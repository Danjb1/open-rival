#pragma once

#include <glm/vec2.hpp>

#include <unordered_set>

namespace Rival {

/** Platform-independent key codes. */
enum class KeyCode
{
    Unknown,

    // Letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Digits
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    // Function keys
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    // Arrow keys
    Up,
    Down,
    Left,
    Right,

    // Other keys
    Space,
    Return,
    Escape,
    Backspace,
    Tab,
    Delete,
    Backtick
};

/** Platform-independent mouse buttons. */
enum class MouseButton
{
    Left,
    Middle,
    Right,
    Other
};

/** Event fired when a key is pressed or released. */
class KeyEvent
{
public:
    explicit KeyEvent(KeyCode key, bool isPress);

    KeyCode key;

    /** True if this is a press, false if it is a release. */
    bool isPress = false;
};

/** Event fired when a mouse button is pressed or released. */
class MouseButtonEvent
{
public:
    MouseButtonEvent(MouseButton button, glm::ivec2 position, bool isPress);

    MouseButton button;

    /** Mouse position, in pixels. */
    glm::ivec2 position;

    /** True if this is a press, false if it is a release. */
    bool isPress = false;
};

/** Event fired when the mouse is moved. */
class MouseMoveEvent
{
public:
    MouseMoveEvent(glm::ivec2 position);

    /** Mouse position, in pixels. */
    glm::ivec2 position;
};

/** Event fired when the mouse wheel is scrolled. */
class MouseWheelEvent
{
public:
    explicit MouseWheelEvent(int delta);

    /** Vertical scroll amount; positive values mean "away from the user".
     * Any platform-specific direction flipping has already been applied. */
    int delta;
};

/**
 * Tracks the current state of the keyboard and mouse.
 *
 * This is kept up to date by feeding it input events, and can be queried at any time.
 */
class Input
{
public:
    void onKeyEvent(const KeyEvent& evt);
    void onMouseButtonEvent(const MouseButtonEvent& evt);
    void onMouseMoveEvent(const MouseMoveEvent& evt);

    /** Releases all keys and mouse buttons, e.g. when the window loses focus. */
    void releaseAll();

    bool isKeyPressed(KeyCode key) const;
    bool isMouseButtonPressed(MouseButton button) const;

    /** Gets the last known mouse position relative to the window, in pixels. */
    glm::ivec2 getMousePosition() const
    {
        return mousePos;
    }

private:
    std::unordered_set<KeyCode> pressedKeys;
    std::unordered_set<MouseButton> pressedMouseButtons;
    glm::ivec2 mousePos { 0, 0 };
};

}  // namespace Rival
