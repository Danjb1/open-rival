#include "pch.h"

#include <iostream>
#include <windows.h>

#include "image-extractor.h"
#include "setup-utils.h"

using namespace ImageExtractor;

int main() {

    if (!createDirectory("images")) {
        std::cerr << "Could not create \"images\" directory\n";
        return -1;
    }

    extractImages(L"IMAGES.dat", "images");
}
