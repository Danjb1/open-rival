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

        int getWidth() const;

        int getHeight() const;

        std::vector<unsigned char>* getData() const;

    private:

        int width;
        int height;
        std::unique_ptr<std::vector<unsigned char>> data;

    };

}

#endif // IMAGE_H
