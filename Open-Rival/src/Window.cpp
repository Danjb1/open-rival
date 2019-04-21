#include "pch.h"
#include "Window.h"

#include <iostream>

#include <SDL.h>

namespace Rival {

    Window::Window(int width, int height, const char* title) {
        this->width = width;
        this->height = height;
        this->title = title;
    }

    void Window::create() {

        windowRef = SDL_CreateWindow(title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN);

        if (windowRef == NULL) {
            std::cerr << "Window could not be created!" << std::endl;
            throw new std::runtime_error(SDL_GetError());
        }
    }

    void Window::destroy() {
        SDL_DestroyWindow(windowRef);
        windowRef = NULL;
    }

    void Window::swapBuffers() {
        SDL_GL_SwapWindow(windowRef);
    }

}
