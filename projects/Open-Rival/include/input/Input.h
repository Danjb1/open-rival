#pragma once

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
    explicit KeyEvent(KeyCode key);

    KeyCode key;
};

/** Event fired when a mouse button is pressed or released. */
class MouseButtonEvent
{
public:
    MouseButtonEvent(MouseButton button, int x, int y);

    MouseButton button;

    /** Mouse position relative to the window, in pixels. */
    int x;
    int y;
};

/** Event fired when the mouse is moved. */
class MouseMoveEvent
{
public:
    MouseMoveEvent(int x, int y);

    /** Mouse position relative to the window, in pixels. */
    int x;
    int y;
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
    void onKeyDown(const KeyEvent& evt);
    void onKeyUp(const KeyEvent& evt);
    void onMouseDown(const MouseButtonEvent& evt);
    void onMouseUp(const MouseButtonEvent& evt);
    void onMouseMoved(const MouseMoveEvent& evt);

    /** Releases all keys and mouse buttons, e.g. when the window loses focus. */
    void releaseAll();

    bool isKeyPressed(KeyCode key) const;
    bool isMouseButtonPressed(MouseButton button) const;

    /** Gets the last known mouse x-position relative to the window, in pixels. */
    int getMouseX() const
    {
        return mouseX;
    }

    /** Gets the last known mouse y-position relative to the window, in pixels. */
    int getMouseY() const
    {
        return mouseY;
    }

private:
    std::unordered_set<KeyCode> pressedKeys;
    std::unordered_set<MouseButton> pressedMouseButtons;
    int mouseX = 0;
    int mouseY = 0;
};

}  // namespace Rival
