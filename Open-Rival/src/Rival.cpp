#include "pch.h"

#include "Rival.h"

namespace Rival {

    bool Rival::initSDL() {

        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create window
        window = SDL_CreateWindow("Rival Realms",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create renderer for window
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
            return false;
        }

        // Initialise renderer colour
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            return false;
        }

        return true;
    }

    bool Rival::loadTitleImage() {

        titleTexture = loadTexture("res/textures/knight.tga");
        if (titleTexture == NULL) {
            std::cout << "Failed to load PNG image!" << std::endl;
            return false;
        }

        return true;
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

    void Rival::initialise() {

        // Start up SDL and create window
        if (!initSDL()) {
            exit();
            throw new std::runtime_error("Failed to initialise");
        }

        // Load title image
        if (!loadTitleImage()) {
            exit();
            throw new std::runtime_error("Failed to load title image");
        }

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

            // Render texture to screen
            SDL_RenderCopy(renderer, titleTexture, NULL, NULL);

            // Update screen
            SDL_RenderPresent(renderer);
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::exit() {

        // Free loaded texture
        SDL_DestroyTexture(titleTexture);
        titleTexture = NULL;

        // Destroy window
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = NULL;
        renderer = NULL;

        // Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

}

/**
 * Entry point for the application.
 */
int main(int argc, char* args[]) {

    try {

        Rival::Rival rival = Rival::Rival();
        rival.initialise();
        rival.start();

    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
