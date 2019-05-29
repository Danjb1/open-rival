#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include <string>
#include <vector>

namespace Rival {

    class Image {

    private:

        int width;
        int height;
        std::unique_ptr<std::vector<unsigned char>> data;

    public:

        Image(const int width, const int height);

        Image(const int width, const int height,
                std::unique_ptr<std::vector<unsigned char>> data);

        int getWidth() const;

        int getHeight() const;

        std::vector<unsigned char>* getData() const;

    };

    /**
     * Loads an image from a file.
     *
     * This is basically the reverse of `write_image` from image-extractor,
     * skipping all the data we don't care about.
     */
    Image loadImage(const std::string filename);

}

#endif // IMAGE_H
