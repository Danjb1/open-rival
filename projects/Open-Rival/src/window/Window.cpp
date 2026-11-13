#include "window/Window.h"

namespace Rival {

Window::Window(int width, int height, const char* title)
    : width(width)
    , height(height)
    , title(title)
    , aspectRatio(static_cast<double>(width) / height)
{
}

void Window::setListener(WindowListener* newListener)
{
    listener = newListener;
}

}  // namespace Rival
