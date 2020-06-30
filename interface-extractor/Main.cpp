#include "pch.h"
#include "interface-extractor.h"

#include <iostream>
#include <windows.h>

using namespace InterfaceExtractor;

/**
 * Attempts to create the given directory.
 */
bool createDirectory(const char* filename) {
    return CreateDirectoryA(filename, NULL)
        || ERROR_ALREADY_EXISTS == GetLastError();
}

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
