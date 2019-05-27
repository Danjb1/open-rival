#include "pch.h"
#include "Image.h"

#include <iostream>

namespace Rival {

    Image::Image(int width, int height) {
        this->width = width;
        this->height = height;

        data = std::shared_ptr<unsigned char>(new unsigned char[width * height]);
        std::fill_n(data.get(), width * height, 0xff);
    }

    Image::Image(int width, int height, std::shared_ptr<unsigned char> data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }

    int Image::getWidth() {
        return width;
    }

    int Image::getHeight() {
        return height;
    }

    std::shared_ptr<unsigned char> Image::getData() {
        return data;
    }

    Image loadImage(std::string filename) {
        std::cout << "Loading: " << filename << "\n";

        std::ifstream ifs(filename, std::ios::binary | std::ios::in);
        if (!ifs) {
            throw std::runtime_error("Failed to load image!");
        }

        // Read sprite dimensions
        ifs.seekg(12);
        int width = ifs.get() | (ifs.get() << 8);
        int height = ifs.get() | (ifs.get() << 8);

        // Read pixel data
        std::shared_ptr<unsigned char> data =
            std::shared_ptr<unsigned char>(new unsigned char[width * height]);
        ifs.seekg(786);
        ifs.read((char*)data.get(), width * height);

        return Image(width, height, data);
    }

};
