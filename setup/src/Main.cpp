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
#include "StringUtils.h"
#include "texture-builder.h"

namespace fs = std::filesystem;

using namespace Rival;

// Configuration
bool shouldCreateOutputDirectories = true;
bool shouldExtractSounds = true;
bool shouldExtractImages = true;
bool shouldExtractInterface = true;
bool shouldBuildTextures = true;
bool shouldCopyVideos = true;

void createOutputDirectories() {
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
        if (!Setup::createDirectory(dir.c_str())) {
            std::cerr << "Could not create directory: " << dir << "\n";
            throw new std::runtime_error("Failed to create output directories");
        }
    }
}

void extractSounds(std::string gameDir) {
    try {
        std::wcout << "Extracting audio\n";
        Setup::extractAudio(gameDir + "\\DATA\\SOUNDS.DAT", "res\\sound");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract sounds: " << e.what() << "\n";
        throw new std::runtime_error("Failed to extract sounds");
    }
}

void extractImages(std::string gameDir) {
    try {
        std::wcout << "Extracting images\n";
        Setup::extractImages(gameDir + "\\DATA\\IMAGES.DAT", "setup\\images\\game");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract images: " << e.what() << "\n";
        throw new std::runtime_error("Failed to extract images");
    }
}

void extractInterface(std::string gameDir) {
    try {
        std::wcout << "Extracting UI images\n";
        Setup::InterfaceExtractor imageExtractor(gameDir + "\\DATA\\Interfac.dat");
        imageExtractor.extractImages("setup\\images\\ui");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to extract UI images: " << e.what() << "\n";
        throw new std::runtime_error("Failed to extract interface");
    }
}

void buildTextures() {
    try {
        std::wcout << "Building textures\n";
        Setup::buildTextures("setup\\definitions\\game", "setup\\images\\game", "res\\textures", false);
        Setup::buildTextures("setup\\definitions\\ui", "setup\\images\\ui", "res\\textures", false);
        Setup::buildTextures("setup\\definitions\\ui\\atlas", "setup\\images\\ui", "res\\textures", true);
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to build textures: " << e.what() << "\n";
        throw new std::runtime_error("Failed to build textures");
    }
}

void copyVideos(std::string videoDir, std::string outputDir) {
    for (const fs::directory_entry& entry : fs::directory_iterator(videoDir)) {
        const fs::path path = entry.path();
        std::filesystem::copy(path, outputDir + "\\" + path.filename().string(),
                std::filesystem::copy_options::update_existing);
    }
}

void copyVideos(std::string gameDir) {
    try {
        copyVideos(gameDir + "\\MOVIES", "res\\video");
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to copy videos: " << e.what() << "\n";
        throw new std::runtime_error("Failed to copy videos");
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

    std::string gameDir;
    bool findDirectoryFromRegistry = true;
    for (int count = 1; count < argc; ++count) {
        if (argv[count] == "-h") {
            std::cout << "-d {directory}: loads all files from {directory}" << '\n';
            return 0;
        }
        if (strcmp("-d", argv[count]) == 0 && count + 1 <= argc) {
            std::string tmpStr(argv[count + 1]);
            gameDir = std::string(tmpStr.begin(), tmpStr.end());
            findDirectoryFromRegistry = false;
            std::cout << "Reading files from directory " << gameDir << '\n';
            ++count;
        }
    }

    if (findDirectoryFromRegistry) {
        std::wcout << "Reading files from registry." << '\n';
        // Read original game directory from registry
        try {
            gameDir = StringUtils::toUtf8(Registry::RegGetString(
                    HKEY_CURRENT_USER,
                    L"Software\\Titus Games\\Rival Realms",
                    L"Game Directory"));
            std::cout << "Found game at: " << gameDir << "\n";
        } catch (const Registry::RegistryError& e) {
            std::cerr << "Failed to find registry entry:\n"
                      << "HKEY_CURRENT_USER\\Software\\Titus Games\\Rival Realms\n"
                      << "Error code: " << e.errorCode() << "\n";
            return -1;
        }
    }

    // Create the output directories
    if (shouldCreateOutputDirectories) {
        createOutputDirectories();
    }

    // Extract sounds
    if (shouldExtractSounds) {
        extractSounds(gameDir);
    }

    // Extract images (game)
    if (shouldExtractImages) {
        extractImages(gameDir);
    }

    // Extract images (ui)
    if (shouldExtractInterface) {
        extractInterface(gameDir);
    }

    // Build textures
    if (shouldBuildTextures) {
        buildTextures();
    }

    // Copy videos
    if (shouldCopyVideos) {
        copyVideos(gameDir);
    }
}
