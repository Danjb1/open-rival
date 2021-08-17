#include "pch.h"
#include "Image.h"

#include <iostream>

#include "BinaryFileReader.h"
#include "Palette.h"

namespace Rival {

    Image::Image(int width, int height, std::uint8_t bgColor)
        : width(width),
          height(height),
          stride(width),
          data(std::make_unique<std::vector<std::uint8_t>>(width * height,
                  bgColor)) {}

    Image::Image(int width, int height,
            std::unique_ptr<std::vector<std::uint8_t>> data)
        : width(width),
          height(height),
          stride(width),
          data(std::move(data)) {}

    Image::Image(int width, int height,
            std::unique_ptr<std::vector<std::uint8_t>> data,
            int stride)
        : width(width),
          height(height),
          stride(stride),
          data(std::move(data)) {}

    Image Image::readImage(const std::string filename) {
        std::cout << "Loading: " << filename << "\n";

        BinaryFileReader reader(filename);

        // Color map specification
        /*uint8_t idLength = */ reader.readByte();
        /*uint8_t colorMapType = */ reader.readByte();
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
        std::unique_ptr<std::vector<std::uint8_t>> data =
                std::make_unique<std::vector<std::uint8_t>>(width * height);
        reader.read(data.get());

        return Image(width, height, std::move(data));
    }

    void Image::copyImage(
            const Image& src,
            const Image& dst,
            const int dstX,
            const int dstY) {

        std::vector<std::uint8_t>* srcData = src.getData();
        std::vector<std::uint8_t>* dstData = dst.getData();

        for (int y = 0; y < src.getHeight(); y++) {
            for (int x = 0; x < src.getWidth(); x++) {
                const int srcIndex = (y * src.getWidth()) + x;
                const int dstIndex = ((dstY + y) * dst.getWidth()) + (dstX + x);

                (*dstData)[dstIndex] = (*srcData)[srcIndex];
            }
        }
    }

};  // namespace Rival
