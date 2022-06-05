#include "pch.h"
#include "Window.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

    Window::Window(int width, int height, const char* title)
        : width(width)
        , height(height)
        , title(title)
        , aspectRatio(static_cast<double>(width) / height)
    {
    }

    Window::~Window()
    {
        // Do nothing!
    }

    void Window::swapBuffers() const
    {
        // Do nothing!
    }

    int Window::getWidth() const
    {
        return width;
    }

    int Window::getHeight() const
    {
        return height;
    }

    double Window::getAspectRatio() const
    {
        return aspectRatio;
    }

}  // namespace Rival
