#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>

#define STBI_ONLY_TGA
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

const int TEXTURE_WIDTH = 2048;
const int TEXTURE_HEIGHT = 2048;

class Image {

private:

    int width;
    int height;
    unsigned char *data;

public:

    Image(int width, int height, unsigned char *data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    unsigned char* getData() {
        return data;
    }

};

Image loadImage(std::string filename) {
    std::cout << "Loading: " << filename << "\n";

    int width, height, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 0);

    if (data == NULL) {
        printf("Failed to load image!\n");
        printf("Error: %s\n", stbi_failure_reason());
        throw std::runtime_error("Error loading image");
    }

    return Image(width, height, data);
}

int main() {

    // Create an empty texture
    unsigned char *data = new unsigned char[TEXTURE_WIDTH * TEXTURE_HEIGHT];
    Image texture = Image(TEXTURE_WIDTH, TEXTURE_HEIGHT, data);

    // Load our definition file
    std::ifstream file("definitions/unit_human_knight.def");
    std::string line;

    while (std::getline(file, line)) {
        Image sprite = loadImage("images/" + line);
        // TODO: add image to texture
        // TODO: start a new row once we hit the max image size
    }
    // TODO: save texture
}
