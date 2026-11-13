#pragma once

#include <string>
#include <vector>

#include "input/Input.h"

namespace Rival {

class WindowListener
{
public:
    virtual ~WindowListener() = default;

    virtual void onExitRequested() = 0;
    virtual void onFocusLost() = 0;
    virtual void onKeyDownEvent(const KeyEvent& evt) = 0;
    virtual void onKeyUpEvent(const KeyEvent& evt) = 0;
    virtual void onMouseDownEvent(const MouseButtonEvent& evt) = 0;
    virtual void onMouseUpEvent(const MouseButtonEvent& evt) = 0;
    virtual void onMouseMoveEvent(const MouseMoveEvent& evt) = 0;
    virtual void onMouseWheelEvent(const MouseWheelEvent& evt) = 0;
};

class Window
{
public:
    Window(int width, int height, const char* title);
    virtual ~Window() = default;

    // Disable moving / copying
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window& operator=(Window&& other) = delete;

    virtual void swapBuffers() const = 0;
    virtual void setPos(int x, int y) = 0;
    virtual void pollEvents() = 0;

    /** Prepares the ImGui platform backend for a new frame. */
    virtual void beginImGuiFrame() = 0;

    void setListener(WindowListener* listener);

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    double getAspectRatio() const
    {
        return aspectRatio;
    }

    bool isVsyncEnabled() const
    {
        return vsyncEnabled;
    }

protected:
    WindowListener* listener = nullptr;
    int width;
    int height;
    double aspectRatio;
    const char* title;
    bool vsyncEnabled { false };
};

}  // namespace Rival
