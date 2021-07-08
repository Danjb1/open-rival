#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

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

        void swapBuffers() const;

        void setIcon(SDL_Surface* surface);

        int getWidth() const;

        int getHeight() const;

        double getAspectRatio() const;

    private:
        SDL_Window* sdlWindow;

        SDL_GLContext glContext;

        const int width;

        const int height;

        const double aspectRatio;

        const char* title;
    };

}  // namespace Rival

#endif  // WINDOW_H
