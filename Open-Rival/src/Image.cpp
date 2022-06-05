#include "pch.h"
#include "Image.h"

#include <iostream>

#include "BinaryFileReader.h"
#include "Palette.h"

namespace Rival {

    Image Image::readImage(const std::string filename)
    {
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
        std::vector<std::uint8_t> data(width * height);
        reader.read(&data);

        return Image::createByMove(width, height, std::move(data));
    }

    Image Image::createEmpty(int width, int height, std::uint8_t bgColor, ImageProperties props)
    {
        std::vector<std::uint8_t> data(width * height, bgColor);
        return Image::createByMove(width, height, std::move(data), props);
    }

    Image Image::createByCopy(int width, int height, std::vector<std::uint8_t>& data, ImageProperties props)
    {
        return Image(width, height, data, props);
    }

    Image Image::createByMove(int width, int height, std::vector<std::uint8_t>&& data, ImageProperties props)
    {
        return Image(width, height, std::move(data), props);
    }

    Image::Image(int width, int height, std::vector<std::uint8_t> data, ImageProperties props)
        : width(width)
        , height(height)
        , data(std::move(data))
        , props(props)
    {
    }

    int Image::getStride() const
    {
        return props.stride == -1 ? width : props.stride;
    }

    void Image::copyImage(const Image& src, Image& dst, const int dstX, const int dstY)
    {

        const std::vector<std::uint8_t>& srcData = src.getData();
        std::vector<std::uint8_t>& dstData = dst.getEditableData();

        for (int y = 0; y < src.getHeight(); y++)
        {
            for (int x = 0; x < src.getWidth(); x++)
            {
                const int srcIndex = (y * src.getWidth()) + x;
                const int dstIndex = ((dstY + y) * dst.getWidth()) + (dstX + x);

                dstData[dstIndex] = srcData[srcIndex];
            }
        }
    }

};  // namespace Rival
