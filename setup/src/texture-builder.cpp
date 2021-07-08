#include "pch.h"
#include "texture-builder.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "setup-utils.h"

namespace fs = std::filesystem;

namespace TextureBuilder {

    // A border between images prevents texture bleeding
    const int borderSize = 1;  // px

    const int maxTextureSize = 2048;

    // Number of colours in the palette
    const int paletteSize = 256;

    ///////////////////////////////////////////////////////////////////////////
    // Comparison functions
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Compare function to sort Images by size - largest first.
     */
    bool compareImagesLargestFirst(Image& img1, Image& img2) {
        // returns true if img1 comes before img2
        int img1Area = img1.getWidth() * img1.getHeight();
        int img2Area = img2.getWidth() * img2.getHeight();
        return img1Area > img2Area;
    }

    /**
     * Compare function to sort Rects by size - smallest first.
     */
    bool compareRectsSmallestFirst(Rect& rect1, Rect& rect2) {
        // returns true if rect1 comes before rect2
        int rect1Area = rect1.width * rect1.height;
        int rect2Area = rect2.width * rect2.height;
        return rect1Area < rect2Area;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Image class
    ///////////////////////////////////////////////////////////////////////////

    Image::Image(std::string filename, int width, int height) {
        this->filename = filename;
        this->width = width;
        this->height = height;

        data = std::shared_ptr<unsigned char>(new unsigned char[width * height]);
        std::fill_n(data.get(), width * height, 0xff);
    }

    Image::Image(std::string filename, int width, int height, const std::shared_ptr<unsigned char> data) {
        this->filename = filename;
        this->width = width;
        this->height = height;
        this->data = data;
    }

    std::string Image::getFilename() const {
        return filename;
    }

    int Image::getWidth() const {
        return width;
    }

    int Image::getHeight() const {
        return height;
    }

    std::shared_ptr<unsigned char> Image::getData() const {
        return data;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Rect class
    ///////////////////////////////////////////////////////////////////////////

    Rect::Rect(int x, int y, int width, int height)
        : x(x),
          y(y),
          width(width),
          height(height) {}

    ///////////////////////////////////////////////////////////////////////////
    // TextureAtlasBuilder class
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Creates an empty TextureAtlasBuilder.
     */
    TextureAtlasBuilder::TextureAtlasBuilder()
        : texWidth(0),
          texHeight(0) {}

    /**
     * Adds an image to the texture being constructed.
     */
    void TextureAtlasBuilder::addImage(const Image& img) {

        // Add some padding around the image
        int imageWidth = img.getWidth() + 2 * borderSize;
        int imageHeight = img.getHeight() + 2 * borderSize;

        // Store this image by its key for later retrieval
        const std::string& filename = img.getFilename();
        const std::string key =
                filename.substr(filename.find_last_of("\\") + 1);
        imagesByKey.insert(std::make_pair(key, img));

        // Ask the builder for an empty space
        Rect dest = findOrMakeEmptyRect(imageWidth, imageHeight);

        // Map this image to its destination Rectangle
        // We use the image filename as the key, but we could use anything, as
        // long as we use the same key to refer to the image when we need it.
        imagePlacements.insert(std::make_pair(key, dest));
    }

    /**
     * Finds an empty Rect of the desired size.
     *
     * If no Rect matches this size exactly, an empty Rect will be subdivided.
     * If no Rect is big enough, the texture will be expanded.
     */
    Rect TextureAtlasBuilder::findOrMakeEmptyRect(
            const int reqWidth, const int reqHeight) {

        // Find the smallest rectangle that fits our required size
        std::sort(
                emptyRects.begin(),
                emptyRects.end(),
                compareRectsSmallestFirst);
        int rectIndex = findRectBiggerThan(
                emptyRects, reqWidth, reqHeight);

        // No free space - need to expand our texture!
        if (rectIndex == -1) {
            rectIndex = expandTextureToFitRect(
                    reqWidth, reqHeight);
        }

        // Make a copy of our target rectangle
        Rect dest = emptyRects[rectIndex];

        // Remove this rectangle from the list of empties
        emptyRects.erase(emptyRects.begin() + rectIndex);

        // Trim the destination Rect to precisely match our required size
        Rect trimmedDest = Rect(
                dest.x,
                dest.y,
                reqWidth,
                reqHeight);

        // Split the leftover space from the destination Rect into new empties
        if (dest.width > reqWidth) {
            // Empty space to the right
            emptyRects.push_back(Rect(
                    dest.x + reqWidth,
                    dest.y,
                    dest.width - reqWidth,
                    reqHeight));
        }
        if (dest.height > reqHeight) {
            // Empty space below
            emptyRects.push_back(Rect(
                    dest.x,
                    dest.y + reqHeight,
                    dest.width,
                    dest.height - reqHeight));
        }

        // TODO: We could improve this by combining adjacent empty Rectangles
        // if their dimensions line up.

        return trimmedDest;
    }

    /**
     * Finds the smallest Rect greater than the given size.
     */
    int TextureAtlasBuilder::findRectBiggerThan(
            const std::vector<Rect>& rects,
            const int minWidth,
            const int minHeight) {
        int rectIndex = -1;
        for (size_t i = 0; i < rects.size(); i++) {
            auto const& rect = rects[i];
            if (rect.width >= minWidth && rect.height >= minHeight) {
                rectIndex = i;
                break;
            }
        }
        return rectIndex;
    }

    /**
     * Expands the TextureAtlas being constructed, to fit a Rect of the given
     * size.
     *
     * Returns the index of the newly-created Rect.
     */
    int TextureAtlasBuilder::expandTextureToFitRect(
            const int reqWidth,
            const int reqHeight) {

        // Remember the previous texture size
        int prevWidth = texWidth;
        int prevHeight = texHeight;

        // Expand our texture downwards
        texHeight += reqHeight;

        // Expand outwards as well if necessary
        if (texWidth < reqWidth) {
            texWidth = reqWidth;
        }

        // We now have space for the desired Rect
        emptyRects.push_back(Rect(
                0,
                prevHeight,
                reqWidth,
                reqHeight));

        // The Rect we just created is the one we want to return
        int rectIndex = emptyRects.size() - 1;

        // Expanding outwards creates empty space to the right of any
        // previously-added Rects
        if (texWidth > prevWidth && prevHeight > 0) {
            emptyRects.push_back(Rect(
                    prevWidth,
                    0,
                    texWidth - prevWidth,
                    prevHeight));
        }

        // Our newly-created Rect may also have empty space to the right,
        // if it does not fill the image width
        if (reqWidth < texWidth) {
            emptyRects.push_back(Rect(
                    reqWidth,
                    prevHeight,
                    texWidth - reqWidth,
                    texHeight - prevHeight));
        }

        return rectIndex;
    }

    ///////////////////////////////////////////////////////////////////////////
    // End of classes
    ///////////////////////////////////////////////////////////////////////////

    /**
     * Loads an image from a file.
     *
     * This is basically the reverse of `write_image` from image-extractor,
     * skipping all the data we don't care about.
     */
    Image loadImage(const std::string& filename) {

        std::ifstream ifs(filename, std::ios::binary | std::ios::in);
        if (!ifs) {
            throw std::runtime_error("Failed to load image: " + filename);
        }

        // Read sprite dimensions
        ifs.seekg(12);
        int width = ifs.get() | (ifs.get() << 8);
        int height = ifs.get() | (ifs.get() << 8);

        // Read pixel data
        std::shared_ptr<unsigned char> data =
                std::shared_ptr<unsigned char>(new unsigned char[width * height]);
        ifs.seekg(1042);
#pragma warning(push)
#pragma warning(disable : 26451)  // Ignore arithmetic overflow warning
        ifs.read((char*) data.get(), width * height);
#pragma warning(pop : 26451)

        return Image(filename, width, height, data);
    }

    void readPalette(
            std::vector<std::uint32_t>& palette,
            const std::string filename) {

        palette.reserve(paletteSize);

        std::ifstream ifs(filename, std::ios::binary | std::ios::in);
        if (!ifs) {
            throw std::runtime_error("Failed to load image for palette: " + filename);
        }

        ifs.seekg(18);

        for (int i = 0; i < paletteSize; ++i) {
            const std::uint8_t blue = ifs.get();
            const std::uint8_t green = ifs.get();
            const std::uint8_t red = ifs.get();
            const std::uint8_t alpha = ifs.get();

            const std::uint32_t col = (red << 24)
                    + (green << 16)
                    + (blue << 8)
                    + alpha;

            palette.push_back(col);
        }
    }

    /**
     * Writes an image to file.
     *
     * Based on `write_image` from image-extractor.
     */
    int writeImage(
            const std::string filename,
            const Image& image,
            const std::vector<std::uint32_t>& palette) {

        // Open file for writing
        FILE* fp = fopen(filename.c_str(), "wb");
        if (!fp) {
            perror(filename.c_str());
            return 0;
        }

        /*
         * Write using TGA format:
         * http://tfc.duke.free.fr/coding/tga_specs.pdf
         */

        fputc(0x00, fp);  // ID Length
        fputc(0x01, fp);  // Color map type
        fputc(0x01, fp);  // Image type (uncompressed, colour-mapped)

        // Color map specification
        fputc(0, fp);  // Index of first entry
        fputc(0x00, fp);
        fputc(0x00, fp);  // Number of entries (256)
        fputc(0x01, fp);
        fputc(32, fp);  // Entry size (32-bit RGBA)

        // Image specification
        fputc(0x00, fp);  // X-origin
        fputc(0x00, fp);
        fputc(0x00, fp);  // Y-origin
        fputc(0x00, fp);
        fputc((uint8_t) image.getWidth(), fp);  // Width
        fputc((uint8_t)(image.getWidth() >> 8), fp);
        fputc((uint8_t) image.getHeight(), fp);  // Height
        fputc((uint8_t)(image.getHeight() >> 8), fp);
        fputc(8, fp);  // Bits per pixel

        // Image descriptor byte
        // (8 = number of alpha bits, bit5: lower-left origin)
        fputc(8 | 0 << 5, fp);

        // Colour map data
        for (int i = 0; i < paletteSize; ++i) {

            const std::uint32_t col = palette[i];
            const std::uint8_t red = (uint8_t)((col & 0xFF000000) >> 24);
            const std::uint8_t green = (uint8_t)((col & 0x00FF0000) >> 16);
            const std::uint8_t blue = (uint8_t)((col & 0x0000FF00) >> 8);
            const std::uint8_t alpha = (uint8_t)(col & 0x000000FF);

            fputc(blue, fp);
            fputc(green, fp);
            fputc(red, fp);
            fputc(alpha, fp);
        }

        // Pixel data
        std::shared_ptr<unsigned char> data = image.getData();
        for (int y = image.getHeight() - 1; y >= 0; y--) {
            for (int x = 0; x < image.getWidth(); x++) {
                const std::uint8_t index = data.get()[x + y * image.getWidth()];
                fputc(index, fp);
            }
        }

        fclose(fp);
        return 1;
    }

    /**
     * Writes an atlas definition to file.
     */
    void writeAtlas(
            const std::string filename,
            TextureAtlasBuilder& builder) {

        // Open the file for writing
        std::ofstream atlasFile;
        atlasFile.open(filename);
        if (!atlasFile.is_open()) {
            throw std::runtime_error("Failed to open atlas file for writing");
        }

        // Write each image placement
        for (auto const& kv : builder.imagePlacements) {
            const std::string& key = kv.first;
            const Rect& target = kv.second;
            const Image& img = builder.imagesByKey.at(key);
            atlasFile << key << " "
                      << target.x + borderSize << " "
                      << target.y + borderSize << " "
                      << target.width - (2 * borderSize) << " "
                      << target.height - (2 * borderSize) << "\n";
        }

        atlasFile.close();
    }

    /**
     * Copies pixels from one image into another.
     */
    void copyImage(
            const Image& src,
            const Image& dst,
            const int dstX,
            const int dstY) {

        std::shared_ptr<unsigned char> srcData = src.getData();
        std::shared_ptr<unsigned char> dstData = dst.getData();

        for (int y = 0; y < src.getHeight(); y++) {
            for (int x = 0; x < src.getWidth(); x++) {

                const int srcIndex = (y * src.getWidth()) + x;
                const int dstIndex = ((dstY + y) * dst.getWidth()) + (dstX + x);

                dstData.get()[dstIndex] = srcData.get()[srcIndex];
            }
        }
    }

    std::vector<Image> readImagesFromDefinitionFile(
            const std::string& imageDir,
            fs::path path,
            bool atlasMode) {

        std::ifstream file(path);
        std::string line;

        int spriteWidth = -1;
        int spriteHeight = -1;

        std::vector<Image> sprites;

        // Load all sprites from the definition file
        while (std::getline(file, line)) {

            Image sprite = loadImage(imageDir + "\\" + line);

            if (atlasMode) {
                sprites.push_back(sprite);
            } else {

                // Set the sprite size based on the first sprite
                if (spriteWidth < 0) {
                    spriteWidth = sprite.getWidth();
                    spriteHeight = sprite.getHeight();
                }

                // Check dimensions against the expected sprite size
                if (sprite.getWidth() > spriteWidth
                        || sprite.getHeight() > spriteHeight) {
                    // Sprite too large
                    throw std::runtime_error("Sprite is too large to fit!");

                } else if (sprite.getWidth() < spriteWidth
                        || sprite.getHeight() < spriteHeight) {
                    // Sprite too small
                    Image resizedSprite = Image(line, spriteWidth, spriteHeight);
                    const int dstX = (spriteWidth - sprite.getWidth()) / 2;
                    const int dstY = (spriteHeight - sprite.getHeight()) / 2;
                    copyImage(sprite, resizedSprite, dstX, dstY);
                    sprites.push_back(resizedSprite);

                } else {
                    // Sprite is ok!
                    sprites.push_back(sprite);
                }
            }
        }

        return sprites;
    }

    void createTextureAtlas(
            const std::string& outputDir,
            fs::path definitionFilename,
            std::vector<Image>& images,
            const std::vector<std::uint32_t>& palette) {

        TextureAtlasBuilder builder;

        // Sort images (largest area first)
        std::sort(images.begin(), images.end(), compareImagesLargestFirst);

        // Find a suitable placement for each image
        for (const auto& img : images) {
            builder.addImage(img);
        }

        // Create the texture
        int texWidth = nextPowerOf2(builder.texWidth);
        int texHeight = nextPowerOf2(builder.texHeight);
        std::cout << "Creating texture of size " << texWidth << ", " << texHeight << "\n";
        Image texture("", texWidth, texHeight);

        // Copy each image onto the texture
        for (auto const& kv : builder.imagePlacements) {
            const std::string& key = kv.first;
            const Rect& target = kv.second;
            const Image& img = builder.imagesByKey.at(key);
            std::cout << "Copying " << img.getFilename() << " to " << target.x << ", " << target.y << "\n";
            copyImage(img, texture, target.x + borderSize, target.y + borderSize);
        }

        // Save the final texture
        writeImage(outputDir + "\\" + definitionFilename.replace_extension(".tga").string(),
                texture,
                palette);

        // Save the atlas definition
        writeAtlas(outputDir + "\\" + definitionFilename.replace_extension(".atlas").string(), builder);
    }

    void createSpritesheetTexture(
            const std::string& outputDir,
            fs::path definitionFilename,
            const std::vector<Image>& sprites,
            const std::vector<std::uint32_t>& palette) {

        // For a spritesheet, all images are the same size
        int spriteWidth = sprites[0].getWidth();
        int spriteHeight = sprites[0].getHeight();

        // Find the optimal texture size:
        // Start with a single long row of sprites, and keep splitting it until
        // we find a suitable size with minimal wasted space
        int tmpWidth = nextPowerOf2(spriteWidth * sprites.size());
        int tmpHeight = spriteHeight;
        int dataSize = tmpWidth * tmpHeight;
        int best = dataSize;
        int txWidth = tmpWidth;
        int txHeight = tmpHeight;

        while (tmpWidth > 256) {

            tmpWidth /= 2;
            tmpHeight *= 2;

            int area = tmpWidth * tmpHeight;
            int wastedSpace = area - dataSize;

            if (txWidth > maxTextureSize || wastedSpace < best) {
                best = wastedSpace;
                txWidth = tmpWidth;
                txHeight = tmpHeight;
            }
        }

        if (txWidth > maxTextureSize || txHeight > maxTextureSize) {
            std::cout << "Optimal size is " << txWidth << " x " << txHeight << "\n";
            throw std::runtime_error("Sprites will not fit!");
        }

        // Create an empty texture
        Image texture = Image("", txWidth, txHeight);

        int x = 0;
        int y = 0;

        // Draw the sprites to the texture
        for (Image sprite : sprites) {
            copyImage(sprite, texture, x, y);

            x += sprite.getWidth();

            if (x >= texture.getWidth()) {
                x = 0;
                y += sprite.getHeight();
            }
        }

        // Save the final texture
        writeImage(outputDir + "\\" + definitionFilename.replace_extension(".tga").string(),
                texture,
                palette);
    }

    void buildTextures(
            std::string definitionsDir,
            std::string imageDir,
            std::string outputDir,
            bool atlasMode) {

        // Process each definition file in the given directory
        for (const fs::directory_entry& entry : fs::directory_iterator(definitionsDir)) {
            const fs::path path = entry.path();
            try {
                std::cout << "Processing: " << path.filename() << "\n";

                // Read images
                std::vector<Image> images = readImagesFromDefinitionFile(
                        imageDir, path, atlasMode);

                // Read palette from the first image
                std::vector<std::uint32_t> palette;
                readPalette(palette, images[0].getFilename());

                // Create texture
                if (atlasMode) {
                    createTextureAtlas(outputDir, path.filename(), images, palette);
                } else {
                    createSpritesheetTexture(outputDir, path.filename(), images, palette);
                }

            } catch (const std::runtime_error& e) {
                std::cout << e.what() << "\n";
                std::cout << "Skipping file: " << path.filename() << "\n";
            }
        }
    }

}  // namespace TextureBuilder
