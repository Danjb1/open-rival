#include "pch.h"
#include "MockWindow.h"

namespace Rival {

    Window::Window(int w, int h)
        : width(width),
          height(height) {}

    void Window::swapBuffers() const {
        // Do nothing
    }

    int Window::getWidth() const {
        return width;
    }

    int Window::getHeight() const {
        return height;
    }

    double Window::getAspectRatio() const {
        return static_cast<double>(width) / height;
    }

}  // namespace Rival
