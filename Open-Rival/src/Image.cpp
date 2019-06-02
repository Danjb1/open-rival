#include "pch.h"
#include "Image.h"

#include <iostream>

namespace Rival {

    Image::Image(const int width, const int height) :
        width(width),
        height(height),
        data(std::make_unique<std::vector<unsigned char>>(width * height, 0xff)) {}

    Image::Image(const int width, const int height,
        std::unique_ptr<std::vector<unsigned char>> data) :
            width(width),
            height(height),
            data(std::move(data)) {}

    int Image::getWidth() const {
        return width;
    }

    int Image::getHeight() const {
        return height;
    }

    std::vector<unsigned char>* Image::getData() const {
        return data.get();
    }

    Image loadImage(const std::string filename) {
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
        std::unique_ptr<std::vector<unsigned char>> data =
                std::make_unique<std::vector<unsigned char>>(width * height);
        ifs.seekg(786);
        ifs.read(reinterpret_cast<char*>(data.get()->data()), width * height);

        return Image(width, height, std::move(data));
    }

};
