#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL.h>

namespace Rival {

    class Window {

    public:

        Window(int width, int height, std::string title);

        void create();

        void destroy();

    private:

        SDL_Window* windowRef;

        int width;

        int height;

        std::string title;

    };

}

#endif // WINDOW_H
