#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>

#define STBI_ONLY_TGA
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

unsigned char* loadImage(std::string filename) {
    std::cout << "Loading: " << filename << "\n";

    int width, height, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 0);

    if (data == NULL) {
        printf("Failed to load image!\n");
        printf("Error: %s\n", stbi_failure_reason());
    }

    return data;
}

int main() {
    std::ifstream file("definitions/unit_human_knight.def");
    std::string line;
    while (std::getline(file, line)) {
        unsigned char *data = loadImage("images/" + line);
        // TODO: add image to texture
        // TODO: start a new row once we hit the max image size
    }
    // TODO: save texture
}
