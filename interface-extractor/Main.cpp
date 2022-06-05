#include "pch.h"

#include <iostream>
#include <windows.h>

#include "interface-extractor.h"
#include "setup-utils.h"

using namespace Rival::Setup;

const std::string outputDir = "images";

/**
 * Entry point for the application.
 */
int main()
{
    if (!createDirectory(outputDir))
    {
        std::cerr << "Could not create directory: " << outputDir << "\n";
        return -1;
    }

    InterfaceExtractor imageExtractor("Interfac.dat");
    imageExtractor.extractImages(outputDir);
}
