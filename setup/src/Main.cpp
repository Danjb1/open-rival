#include "pch.h"

#define WIN32_LEAN_AND_MEAN

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <windows.h>

#include "audio-extractor.h"
#include "image-extractor.h"
#include "interface-extractor.h"
#include "registry.h"
#include "setup-utils.h"
#include "texture-builder.h"

namespace fs = std::filesystem;

void copyVideos(std::wstring videoDir, std::string outputDir) {
    for (const fs::directory_entry& entry : fs::directory_iterator(videoDir)) {
        const fs::path path = entry.path();
        std::filesystem::copy(path, outputDir + "\\" + path.filename().string(),
                std::filesystem::copy_options::update_existing);
    }
}

/**
 * Setup program to prepare the game's assets.
 *
 * This is pretty rough around the edges, and would benefit from better error
 * handling, more thorough logging, and consistent handling of paths.
 *
 * The code is very non-portable.
 */
int main(int argc, char* argv[]) {

    std::wstring gameDir;
    bool findDirectoryFromRegistry = true;
    for (int count = 1; count < argc; ++count) {
        if (argv[count] == "-h") {
            std::cout << "-d {directory}: loads all files from {directory}" << '\n';
            return 0;
        }
        if (strcmp("-d", argv[count]) == 0 && count + 1 <= argc) {
            std::string tmpStr(argv[count + 1]);
            gameDir = std::wstring(tmpStr.begin(), tmpStr.end());
            findDirectoryFromRegistry = false;
            std::wcout << "Reading files from directory " << gameDir << '\n';
            ++count;
        }
    }

    if (findDirectoryFromRegistry) {
        std::wcout << "Reading files from registry." << '\n';
        // Read original game directory from registry
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
    }

    // Create the output directories
    std::wcout << "Creating output directories\n";
    std::vector<std::string> directories = {
        "setup\\images",
        "setup\\images\\game",
        "setup\\images\\ui",
        "res\\sound",
        "res\\textures",
        "res\\video",
    };
    for (auto const& dir : directories) {
        if (!createDirectory(dir.c_str())) {
            std::cerr << "Could not create directory: " << dir << "\n";
            return -1;
        }
    }

    // Extract sounds
    try {
        std::wcout << "Extracting audio\n";
        AudioExtractor::extractAudio(gameDir + L"\\DATA\\SOUNDS.DAT", "res\\sound");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract sounds: " << e.what() << "\n";
        return -1;
    }

    // Extract images (game)
    try {
        std::wcout << "Extracting images\n";
        ImageExtractor::extractImages(gameDir + L"\\DATA\\IMAGES.DAT", "setup\\images\\game");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract images: " << e.what() << "\n";
        return -1;
    }

    // Extract images (ui)
    try {
        std::wcout << "Extracting UI images\n";
        InterfaceExtractor::extractUiImages(gameDir + L"\\DATA\\Interfac.DAT", "setup\\images\\ui");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract UI images: " << e.what() << "\n";
        return -1;
    }

    // Build textures
    try {
        std::wcout << "Building textures\n";

        // Game
        TextureBuilder::buildTextures("setup\\definitions\\game", "setup\\images\\game", "res\\textures", false);

        // UI
        TextureBuilder::buildTextures("setup\\definitions\\ui\\atlas", "setup\\images\\ui", "res\\textures", true);

    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to build textures: " << e.what() << "\n";
        return -1;
    }

    // Copy videos
    try {
        copyVideos(gameDir + L"\\MOVIES", "res\\video");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to copy videos: " << e.what() << "\n";
        return -1;
    }
}
