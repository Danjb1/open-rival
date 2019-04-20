#include "pch.h"
#include "Window.h"

#include <iostream>

#include <SDL.h>

namespace Rival {

    Window::Window(int width, int height, std::string title) {
        this->width = width;
        this->height = height;
        this->title = title;
    }

    void Window::create() {

        windowRef = SDL_CreateWindow(title.c_str(),
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

}
