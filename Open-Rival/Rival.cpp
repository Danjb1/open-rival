#include "pch.h"

#include "Rival.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
SDL_Window* window = NULL;

// The window renderer
SDL_Renderer* renderer = NULL;

// Current displayed texture
SDL_Texture* titleTexture = NULL;

bool init() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		cout << "Warning: Linear texture filtering not enabled!" << endl;
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

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << endl;
		return false;
	}

	// Initialise renderer colour
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
		return false;
	}

	return true;
}

bool loadTitleImage() {

	titleTexture = loadTexture("gfx/title.png");
	if (titleTexture == NULL) {
		cout << "Failed to load PNG image!" << endl;
		return false;
	}

	return true;
}

SDL_Texture* loadTexture(std::string path) {

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
		return NULL;
	}

	// Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL) {
		cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
		return NULL;
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

void close() {

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

		// Clear screen
		SDL_RenderClear(renderer);

		// Render texture to screen
		SDL_RenderCopy(renderer, titleTexture, NULL, NULL);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Free resources and close SDL
	close();

	return 0;
}
