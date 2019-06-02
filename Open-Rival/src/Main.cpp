#include "pch.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

#include "Rival.h"

/**
 * Entry point for the application.
 */
int main() {

    Rival::Rival rival;

    try {
        rival.initialize();
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to initialise\n";
        std::cerr << e.what() << "\n";
        return 1;
    }

    try {
        rival.start();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error during gameplay\n";
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
