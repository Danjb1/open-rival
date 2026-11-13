#pragma once

#include <string>
#include <vector>

namespace Rival {

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
    const int width;
    const int height;
    const double aspectRatio;
    const char* title;
    bool vsyncEnabled { false };
};

}  // namespace Rival
