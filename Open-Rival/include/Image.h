#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace Rival {

    class Image {

    public:
        static Image readImage(const std::string filename);

        /**
         * Creates a blank image.
         */
        Image(int width, int height, std::uint8_t bgColor);

        /**
         * Creates an image by taking ownership of some data.
         */
        Image(int width, int height,
                std::unique_ptr<std::vector<std::uint8_t>> data);

        /**
         * Creates an image by taking ownership of some data, with a custom
         * stride.
         */
        Image(int width, int height,
                std::unique_ptr<std::vector<std::uint8_t>> data,
                int stride);

        int getWidth() const { return width; }

        int getHeight() const { return height; }

        int getStride() const { return stride; }

        std::vector<std::uint8_t>* getData() const { return data.get(); };

        /**
         * Copies pixels from one image into another.
         */
        static void copyImage(
                const Image& src,
                const Image& dst,
                const int dstX,
                const int dstY);

    private:
        int width;
        int height;

        /**
         * Stride used to separate rows of the image within the data buffer.
         */
        int stride;

        std::unique_ptr<std::vector<std::uint8_t>> data;
    };

}  // namespace Rival

#endif  // IMAGE_H
