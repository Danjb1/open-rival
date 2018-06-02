#include "Rival.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
SDL_Window* window = NULL;

// The surface contained by the window
SDL_Surface* screenSurface = NULL;

// Title image to be rendered
SDL_Surface* titleImage = NULL;

bool init() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	// Get the surface contained by the window
	screenSurface = SDL_GetWindowSurface(window);

	return true;
}

bool loadTitleImage() {

	titleImage = loadSurface("gfx/title.png");
	if (titleImage == NULL) {
		printf("Failed to load PNG image!\n");
		return false;
	}

	return true;
}

SDL_Surface* loadSurface(std::string path) {

	// The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	// Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
	if (optimizedSurface == NULL) {
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return optimizedSurface;
}

void close() {

	// Free loaded image
	SDL_FreeSurface(titleImage);
	titleImage = NULL;

	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[]) {

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
		close();
		return 1;
	}

	// Load title image
	if (!loadTitleImage()) {
		printf("Failed to load title image!\n");
		close();
		return 1;
	}

	// Main loop flag
	bool quit = false;

	// Event handler
	SDL_Event e;

	// While application is running
	while (!quit) {

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {

			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Apply the image
		SDL_BlitSurface(titleImage, NULL, screenSurface, NULL);

		// Update the surface
		SDL_UpdateWindowSurface(window);
	}

	// Free resources and close SDL
	close();

	return 0;
}
