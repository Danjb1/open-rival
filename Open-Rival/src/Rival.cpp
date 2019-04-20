#include "pch.h"
#include "Rival.h"

/**
 * Entry point for the application.
 */
int main(int argc, char* args[]) {

    Rival::Rival rival = Rival::Rival();

    try {
        rival.initialise();
    } catch (std::runtime_error e) {
        std::cerr << "Failed to initialise" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        rival.start();
    } catch (std::runtime_error e) {
        std::cerr << "Error during gameplay" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

namespace Rival {

    void Rival::initialise() {
        initSDL();
        window = createWindow();
    }

    void Rival::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize!" << std::endl;
            throw new std::runtime_error(SDL_GetError());
        }
    }

    std::unique_ptr<Window> Rival::createWindow() {
        Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        window.create();

        return std::make_unique<Window>(window);
    }

    void Rival::start() {

        // Event handler
        SDL_Event e;

        // Run our game loop until the application is exitd
        bool exiting = false;
        while (!exiting) {

            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {

                // User requests exiting
                if (e.type == SDL_QUIT) {
                    exiting = true;
                } else if (e.type == SDL_KEYDOWN) {
                    keyDown(e.key.keysym.sym);
                }
            }

            // Clear screen
            SDL_RenderClear(renderer);

            // Update screen
            SDL_RenderPresent(renderer);
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::exit() {

        // Destroy window
        SDL_DestroyRenderer(renderer);
        window->destroy();
        renderer = NULL;

        // Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Texture* Rival::loadTexture(std::string path) {

        // Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            std::cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << std::endl;
            return NULL;
        }

        // Create texture from surface pixels
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            std::cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << std::endl;
            return NULL;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);

        return newTexture;
    }

    void Rival::keyDown(SDL_Keycode keyCode) {
        switch (keyCode) {
        case SDLK_UP:
            std::cout << "UP\n";
            break;

        case SDLK_DOWN:
            std::cout << "DOWN\n";
            break;

        case SDLK_LEFT:
            std::cout << "LEFT\n";
            break;

        case SDLK_RIGHT:
            std::cout << "RIGHT\n";
            break;

        default:
            break;
        }
    }

}
