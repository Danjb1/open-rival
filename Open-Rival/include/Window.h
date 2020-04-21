#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>

namespace Rival {

    class Window {

    public:

        Window(int width, int height, const char* title);

        ~Window();

        // Disable moving / copying
        Window(const Window& other) = delete;
        Window(Window&& other) = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) = delete;

        void use();

        void swapBuffers() const;

    private:

        SDL_Window* windowRef;

        SDL_GLContext glContext;

        const int width;

        const int height;

        const char* title;

    };

}

#endif // WINDOW_H
