#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>

namespace Rival {

    class Window {

    private:

        SDL_Window* windowRef;

        SDL_GLContext glContext;

        int width;

        int height;

        const char* title;

    public:

        Window(const int width, const int height, const char* title);

        ~Window();

        void use();

        void swapBuffers() const;

    };

}

#endif // WINDOW_H
