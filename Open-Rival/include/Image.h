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

        Image(int width, int height);

        Image(int width, int height,
                std::unique_ptr<std::vector<unsigned char>> data);

        int getWidth();

        int getHeight();

        std::vector<unsigned char>* getData();

    };

    /**
     * Loads an image from a file.
     *
     * This is basically the reverse of `write_image` from image-extractor,
     * skipping all the data we don't care about.
     */
    Image loadImage(std::string filename);

}

#endif // IMAGE_H
