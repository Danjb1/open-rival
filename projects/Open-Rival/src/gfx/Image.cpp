#include "gfx/Image.h"

#include <utility>  // std::move

#include "gfx/Palette.h"
#include "utils/BinaryFileReader.h"
#include "utils/LogUtils.h"

namespace Rival {

Image Image::readImage(const std::string& filename)
{
    LOG_TRACE("Reading image: {}", filename);

    BinaryFileReader reader(filename);

    // Color map specification
    /*uint8_t idLength = */ reader.readByte();
    uint8_t colorMapType = reader.readByte();
    uint8_t imageType = reader.readByte();
    /*uint16_t firstEntryIndex = */ reader.readShort();
    uint16_t numEntries = reader.readShort();
    uint8_t entrySize = reader.readByte();

    // Image specification
    /*uint16_t xOrigin = */ reader.readShort();
    /*uint16_t yOrigin = */ reader.readShort();
    std::uint16_t width = reader.readShort();
    std::uint16_t height = reader.readShort();
    uint8_t bpp = reader.readByte();
    uint8_t imageDescriptor = reader.readByte();

    // Sanity check image format
    if (colorMapType != 1 || imageType != 1 || bpp != 8)
    {
        throw std::runtime_error("Unsupported image type: " + filename);
    }
    if (numEntries != Palette::numColors)
    {
        throw std::runtime_error("Image contains invalid palette: " + filename);
    }

    // Color palette (we can ignore this since we know the palettes ahead of time)
    int numChannels = entrySize / 8;  // entrySize is 16, 24 or 32
    reader.skip(Palette::numColors * numChannels);

    // Pixel data
    std::vector<std::uint8_t> data(width * height);
    reader.read(&data);

    // HACK: Fix reverse palette in files exported by Photoshop
    auto bytesRemaining = reader.getBytesRemaining();
    if (static_cast<int>(bytesRemaining) == tgaFooterSize)
    {
        /* This is a bit of a hack, but we use the presence of the TGA footer to detect files exported by Photoshop (as
         * opposed to files extracted programmatically from the game assets). A less hacky approach would be to peek at
         * the palette to see if it is reversed.
         *
         * For whatever reason, files exported by Photoshop have their palettes reversed, so we fix this by inverting
         * the value of every pixel.
         */
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            data[i] = 255 - data[i];
        }
    }

    // Fix images with a lower-left origin
    if ((imageDescriptor & (1 << 5)) == 0)
    {
        // Bit 5 of the image descriptor bytes describes the vertical origin (top or bottom).
        // We expect images to have a top-left origin, so if this is not the case, we need to fix it - otherwise the
        // image will be upside-down.
        const auto lastRow = height - 1;
        for (auto y = 0; y < height / 2; ++y)
        {
            for (auto x = 0; x < width; ++x)
            {
                std::size_t i = y * width + x;
                std::size_t j = (lastRow - y) * width + x;
                std::swap(data[i], data[j]);
            }
        }
    }

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
    std::vector<std::uint8_t>& dstData = dst.getMutableData();

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

std::uint8_t Image::getPixel(int x, int y) const
{
    const int stride = getStride();
    return data[x + y * stride];
}

void Image::setPixel(int x, int y, std::uint8_t col)
{
    const int stride = getStride();
    data[x + y * stride] = col;
}

};  // namespace Rival
