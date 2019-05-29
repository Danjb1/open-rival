#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>

namespace Rival {

    class Window {

    public:

        Window(const int width, const int height, const char* title);

        void create();

        void destroy();

        void use();

        void swapBuffers() const;

    private:

        SDL_Window* windowRef;

        SDL_GLContext glContext;

        int width;

        int height;

        const char* title;

    };

}

#endif // WINDOW_H
