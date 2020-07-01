#include "image-extractor.h"

#include <iostream>
#include <windows.h>

using namespace ImageExtractor;

/**
 * Attempts to create the given directory.
 */
bool createDirectory(const char* filename) {
    return CreateDirectoryA(filename, NULL)
            || ERROR_ALREADY_EXISTS == GetLastError();
}

int main() {

    if (!createDirectory("images")) {
        std::cerr << "Could not create \"images\" directory\n";
        return -1;
    }

    extractImages(L"IMAGES.dat", "images");
}
