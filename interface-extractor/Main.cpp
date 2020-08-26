#include "pch.h"

#include <iostream>
#include <windows.h>

#include "interface-extractor.h"
#include "setup-utils.h"

using namespace InterfaceExtractor;

/**
 * Entry point for the application.
 */
int main() {

    if (!createDirectory("images")) {
        std::cerr << "Could not create \"images\" directory\n";
        return -1;
    }

    extractUiImages(L"Interfac.dat", "images");
}
