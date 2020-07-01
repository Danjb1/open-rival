#include "pch.h"
#include "Window.h"

#include <iostream>

#include <SDL.h>

namespace Rival {

    Window::Window(int width, int height, const char* title)
        : width(width),
          height(height),
          title(title) {

        windowRef = SDL_CreateWindow(title,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width,
                height,
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (windowRef == nullptr) {
            std::cerr << "Window could not be created!\n";
            throw std::runtime_error(SDL_GetError());
        }
    }

    Window::~Window() {
        SDL_DestroyWindow(windowRef);
        windowRef = nullptr;
    }

    void Window::use() {

        glContext = SDL_GL_CreateContext(windowRef);

        if (glContext == nullptr) {
            std::cerr << "OpenGL context could not be created!\n";
            throw std::runtime_error(SDL_GetError());
        }
    }

    void Window::swapBuffers() const {
        SDL_GL_SwapWindow(windowRef);
    }

}  // namespace Rival
