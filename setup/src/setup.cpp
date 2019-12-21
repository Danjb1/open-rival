#include "pch.h"

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <windows.h>

#include "audio-extractor.h"
#include "image-extractor.h"
#include "registry.h"
#include "setup-utils.h"

int main() {

    // Read original game directory from registry
    std::wstring gameDir;
    try {
        gameDir = Registry::RegGetString(
                HKEY_CURRENT_USER,
                L"Software\\Titus Games\\Rival Realms",
                L"Game Directory");
        std::wcout << "Found game at: " << gameDir << "\n";
    } catch (const Registry::RegistryError& e) {
        std::cerr << "Failed to find registry entry:\n"
            << "HKEY_CURRENT_USER\\Software\\Titus Games\\Rival Realms\n"
            << "Error code: " << e.errorCode() << "\n";
        return -1;
    }

    // Create the output directories
    std::wcout << "Creating output directories\n";
    if (!create_directory("images")) {
        std::cerr << "Could not create \"images\" directory\n";
        return -1;
    }
    if (!create_directory("..\\res\\sound")) {
        std::cerr << "Could not create \"sound\" directory\n";
        return -1;
    }
    if (!create_directory("..\\res\\textures")) {
        std::cerr << "Could not create \"textures\" directory\n";
        return -1;
    }
    if (!create_directory("..\\res\\video")) {
        std::cerr << "Could not create \"video\" directory\n";
        return -1;
    }

    // Extract sounds
    try {
        std::wcout << "Extracting audio\n";
        extractAudio(gameDir + L"\\DATA\\SOUNDS.DAT", "..\\res\\sound");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract sounds: " << e.what() << "\n";
        return -1;
    }

    // Extract images
    try {
        std::wcout << "Extracting images\n";
        extractImages(gameDir + L"\\DATA\\IMAGES.DAT", "images");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract images: " << e.what() << "\n";
        return -1;
    }

    // Build textures
    // TODO

    // Copy videos
    // TODO
}
