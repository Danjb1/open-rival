#include "pch.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

#include "Rival.h"
#include "ScenarioReader.h"

/**
 * Entry point for the application.
 */
int main() {

    Rival::ScenarioReader reader("D://rivalrealms/MAPS/1.sco");
    //Rival::ScenarioReader reader("D://rivalrealms/MAPS/4PBATT.sco");
    return 1;

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
