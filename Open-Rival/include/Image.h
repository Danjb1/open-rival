#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace Rival {

    class Image {

    public:
        /**
         * Loads an image from a file.
         *
         * This is basically the reverse of `write_image` from image-extractor,
         * skipping all the data we don't care about.
         */
        static Image loadImage(const std::string filename);

        Image(int width, int height);

        Image(int width, int height,
                std::unique_ptr<std::vector<unsigned char>> data);

        int getWidth() const { return width; }

        int getHeight() const { return height; }

        std::vector<unsigned char>* getData() const { return data.get(); };

    private:
        int width;
        int height;
        std::unique_ptr<std::vector<unsigned char>> data;
    };

}  // namespace Rival

#endif  // IMAGE_H
