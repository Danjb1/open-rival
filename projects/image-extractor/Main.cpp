#include <windows.h>

#include <iostream>

#include "image-extractor.h"
#include "setup-utils.h"

using namespace Rival::Setup;

int main()
{
    if (!createDirectory("images"))
    {
        std::cerr << "Could not create \"images\" directory\n";
        return -1;
    }

    extractImages("IMAGES.dat", "images");
}
