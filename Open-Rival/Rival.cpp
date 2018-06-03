#include "Rival.h"

using namespace std;

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
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
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
		cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
		return false;
	}

	// Get the surface contained by the window
	screenSurface = SDL_GetWindowSurface(window);

	return true;
}

bool loadTitleImage() {

	titleImage = loadSurface("gfx/title.png");
	if (titleImage == NULL) {
		cout << "Failed to load PNG image!" << endl;
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
		cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
		return NULL;
	}

	// Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
	if (optimizedSurface == NULL) {
		cout << "Unable to optimise image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
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

void keyDown(SDL_Keycode keyCode) {
	switch (keyCode) {
	case SDLK_UP:
		cout << "UP\n";
		break;

	case SDLK_DOWN:
		cout << "DOWN\n";
		break;

	case SDLK_LEFT:
		cout << "LEFT\n";
		break;

	case SDLK_RIGHT:
		cout << "RIGHT\n";
		break;

	default:
		break;
	}
}

int main(int argc, char* args[]) {

	// Start up SDL and create window
	if (!init()) {
		cout << "Failed to initialise!\n";
		close();
		return 1;
	}

	// Load title image
	if (!loadTitleImage()) {
		cout << "Failed to load title image!\n";
		close();
		return 1;
	}

	// Event handler
	SDL_Event e;

	// Run our game loop until the application is closed
	bool exiting = false;
	while (!exiting) {

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {

			// User requests exiting
			if (e.type == SDL_QUIT) {
				exiting = true;
			} else if (e.type == SDL_KEYDOWN) {
				keyDown(e.key.keysym.sym);
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
