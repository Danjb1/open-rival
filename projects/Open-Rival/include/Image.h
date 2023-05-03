#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace Rival {

struct ImageProperties
{
    /**
     * Stride used to separate rows of the image within the data buffer.
     *
     * If this is set to -1, the image width is used instead.
     */
    int stride = -1;
};

/**
 * An image consisting of 1 byte per pixel.
 *
 * Normally, we use these values as a lookup into a separate palette texture, since the game only uses 256 colors.
 */
class Image
{
public:
    static Image readImage(const std::string& filename);

    /**
     * Creates an Image by copying some existing data.
     */
    static Image createEmpty(int width, int height, std::uint8_t bgColor, ImageProperties props = {});

    /**
     * Creates an Image by copying some existing data.
     */
    static Image createByCopy(int width, int height, std::vector<std::uint8_t>& data, ImageProperties props = {});

    /**
     * Creates an Image by taking ownership of some existing data.
     */
    static Image createByMove(int width, int height, std::vector<std::uint8_t>&& data, ImageProperties props = {});

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    int getStride() const;

    std::vector<std::uint8_t>& getMutableData()
    {
        return data;
    };

    const std::vector<std::uint8_t>& getData() const
    {
        return data;
    };

    /**
     * Copies pixels from one image into another.
     */
    static void copyImage(const Image& src, Image& dst, const int dstX, const int dstY);

private:
    int width;
    int height;
    ImageProperties props;
    std::vector<std::uint8_t> data;

    Image(int width, int height, std::vector<std::uint8_t> data, ImageProperties props);
};

}  // namespace Rival
