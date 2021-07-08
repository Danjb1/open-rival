#include "pch.h"
#include "Image.h"

#include <iostream>

#include "BinaryFileReader.h"
#include "Palette.h"

namespace Rival {

    Image::Image(int width, int height)
        : width(width),
          height(height),
          data(std::make_unique<std::vector<unsigned char>>(width * height, '\xff')) {}

    Image::Image(int width, int height,
            std::unique_ptr<std::vector<unsigned char>> data)
        : width(width),
          height(height),
          data(std::move(data)) {}

    Image Image::loadImage(const std::string filename) {
        std::cout << "Loading: " << filename << "\n";

        BinaryFileReader reader(filename);

        // Color map specification
        /*uint8_t idLength = */ reader.readByte();
        /*uint8_t colourMapType = */ reader.readByte();
        /*uint8_t imageType = */ reader.readByte();
        /*uint16_t firstEntryIndex = */ reader.readShort();
        /*uint16_t numEntries = */ reader.readShort();
        /*uint8_t entrySize = */ reader.readByte();

        // Image specification
        /*uint16_t xOrigin = */ reader.readShort();
        /*uint16_t yOrigin = */ reader.readShort();
        std::uint16_t width = reader.readShort();
        std::uint16_t height = reader.readShort();
        /*uint8_t bpp = */ reader.readByte();
        /*uint8_t imageDescriptor = */ reader.readByte();

        // Color palette
        reader.skip(Palette::paletteBytes);

        // Pixel data
        std::unique_ptr<std::vector<unsigned char>> data =
                std::make_unique<std::vector<unsigned char>>(width * height);
        reader.read(data.get());

        return Image(width, height, std::move(data));
    }

};  // namespace Rival
