#include "application/Window.h"

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

}  // namespace Rival
