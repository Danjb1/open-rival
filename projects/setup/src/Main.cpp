#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include "utils/LogUtils.h"
#include "utils/StringUtils.h"
#include "ImageGen.h"
#include "audio-extractor.h"
#include "image-extractor.h"
#include "interface-extractor.h"
#include "registry.h"
#include "setup-utils.h"
#include "texture-builder.h"

namespace fs = std::filesystem;

using namespace Rival;

// Configuration
bool shouldCreateOutputDirectories = true;
bool shouldExtractSounds = true;
bool shouldExtractImages = true;
bool shouldExtractInterface = true;
bool shouldMakeImages = true;
bool shouldBuildTextures = true;
bool shouldCopyVideos = true;

void createOutputDirectories()
{
    LOG_INFO("Creating output directories");
    std::vector<std::string> directories = {
        "setup\\images",  //
        "res\\sound",     //
        "res\\textures",  //
        "res\\video"      //
    };
    for (const auto& dir : directories)
    {
        if (!Setup::createDirectory(dir.c_str()))
        {
            LOG_ERROR("Could not create directory {}", dir);
            throw std::runtime_error("Failed to create output directories");
        }
    }
}

void extractSounds(std::string gameDir)
{
    LOG_INFO("Extracting audio");
    Setup::extractAudio(gameDir + "\\DATA\\SOUNDS.DAT", "res\\sound");
}

void extractImages(std::string gameDir)
{
    LOG_INFO("Extracting images");
    Setup::extractImages(gameDir + "\\DATA\\IMAGES.DAT", "setup\\images");
}

void extractInterface(std::string gameDir)
{
    LOG_INFO("Extracting UI images");
    Setup::InterfaceExtractor imageExtractor(gameDir + "\\DATA\\Interfac.dat");
    imageExtractor.extractImages("setup\\images");
}

void makeImages(std::string gameDir)
{
    LOG_INFO("Making procedural images");
    Setup::makeProceduralImages("setup\\images");
}

void buildTextures()
{
    LOG_INFO("Building textures");
    Setup::buildTextures("setup\\definitions\\game", "setup\\images", "res\\textures");
    Setup::buildTextures("setup\\definitions\\ui", "setup\\images", "res\\textures");
}

void copyVideos(std::string videoDir, std::string outputDir)
{
    for (const fs::directory_entry& entry : fs::directory_iterator(videoDir))
    {
        const fs::path path = entry.path();
        std::filesystem::copy(
                path, outputDir + "\\" + path.filename().string(), std::filesystem::copy_options::update_existing);
    }
}

void copyVideos(std::string gameDir)
{
    LOG_INFO("Copying videos");
    copyVideos(gameDir + "\\MOVIES", "res\\video");
}

/**
 * Setup program to prepare the game's assets.
 *
 * This is pretty rough around the edges, and would benefit from better error
 * handling, more thorough logging, and consistent handling of paths.
 *
 * The code is very non-portable.
 */
int main(int argc, char* argv[])
{
    // Parse command-line arguments
    std::string gameDir;
    bool findDirectoryFromRegistry = true;
    bool isDebugMode = false;
    for (int count = 1; count < argc; ++count)
    {
        std::string arg(argv[count]);
        if (arg == "-h")
        {
            std::cout << "-d {directory}: loads all files from {directory}" << '\n';
            return 0;
        }
        if (arg == "-d" && count + 1 <= argc)
        {
            gameDir = std::string((argv[count + 1]));
            findDirectoryFromRegistry = false;
            ++count;  // skip next argument (directory)
        }
        if (arg == "--debug")
        {
            isDebugMode = true;
        }
    }

    const std::string logLevel = isDebugMode ? "debug" : "info";
    LogUtils::initLogging(logLevel, /* logToConsole = */ true, /* logToFile = */ true, "setup");

    if (findDirectoryFromRegistry)
    {
        // Read original game directory from registry
        LOG_INFO("Looking for game directory in registry");
        try
        {
            gameDir = StringUtils::toUtf8(Registry::RegGetString(HKEY_CURRENT_USER,  //
                    L"Software\\Titus Games\\Rival Realms",                          //
                    L"Game Directory"));
            LOG_INFO("Found game directory: {}", gameDir);
        }
        catch (const Registry::RegistryError& e)
        {
            LOG_ERROR("Failed to find registry entry:\n"
                      "HKEY_CURRENT_USER\\Software\\Titus Games\\Rival Realms\n"
                      "Error code: {}",
                    e.errorCode());
            return -1;
        }
    }
    else
    {
        LOG_INFO("Reading files from user-supplied directory: {}", gameDir);
    }

    try
    {
        // Create the output directories
        if (shouldCreateOutputDirectories)
        {
            createOutputDirectories();
        }

        // Extract sounds
        if (shouldExtractSounds)
        {
            extractSounds(gameDir);
        }

        // Extract images (game)
        if (shouldExtractImages)
        {
            extractImages(gameDir);
        }

        // Extract images (ui)
        if (shouldExtractInterface)
        {
            extractInterface(gameDir);
        }

        // Procedural images
        if (shouldMakeImages)
        {
            makeImages(gameDir);
        }

        // Build textures
        if (shouldBuildTextures)
        {
            buildTextures();
        }

        // Copy videos
        if (shouldCopyVideos)
        {
            copyVideos(gameDir);
        }
    }
    catch (const std::runtime_error& e)
    {
        LOG_ERROR("Error during setup: {}", e.what());
    }
}
