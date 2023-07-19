/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "application/Window.h"

namespace Rival {

Window::Window(int width, int height, const char* title)
    : width(width)
    , height(height)
    , title(title)
    , aspectRatio(static_cast<double>(width) / height)
{
}

Window::~Window() {}

void Window::swapBuffers() const {}

void Window::setIcon(SDL_Surface* /*surface*/) {}

}  // namespace Rival
