#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

// Starts up SDL and creates window
bool init();

// Loads the title image
bool loadTitleImage();

// Loads a surface from a file
SDL_Surface* loadSurface(std::string path);

// Frees media and shuts down SDL
void close();

// Handles keyDown events
void keyDown(SDL_Keycode keyCode);
