#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>

namespace Rival {

    class Window {

    public:

        Window(int width, int height, const char* title);

        void create();

        void destroy();

        void swapBuffers();

    private:

        SDL_Window* windowRef;

        int width;

        int height;

        const char* title;

    };

}

#endif // WINDOW_H
