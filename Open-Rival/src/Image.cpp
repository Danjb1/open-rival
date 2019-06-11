#include "pch.h"
#include "Image.h"

#include <iostream>

#include "BinaryFileReader.h"

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

        BinaryFileReader reader(filename);

        // Color map specification
        uint8_t idLength = reader.readByte();
        uint8_t colourMapType = reader.readByte();
        uint8_t imageType = reader.readByte();
        uint16_t firstEntryIndex = reader.readShort();
        uint16_t numEntries = reader.readShort();
        uint8_t entrySize = reader.readByte();

        // Image specification
        uint16_t xOrigin = reader.readShort();
        uint16_t yOrigin = reader.readShort();
        uint16_t width = reader.readShort();
        uint16_t height = reader.readShort();
        uint8_t bpp = reader.readByte();

        // Skip some... (TODO)
        reader.skip(769);

        // Read pixel data
        std::unique_ptr<std::vector<unsigned char>> data =
                std::make_unique<std::vector<unsigned char>>(width * height);
        reader.read(data.get());

        return Image(width, height, std::move(data));
    }

};
