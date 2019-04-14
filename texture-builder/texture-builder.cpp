#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int MAX_TEXTURE_SIZE = 2048;

// Number of colours in the palette
const int PALETTE_SIZE = 256;

// The game's colour palette.
// The last colour (index 0xff) is used for transparent areas;
// this colour is never written by the code in "IMAGES.DAT".
const uint32_t PALETTE[PALETTE_SIZE] = {
    0x000000ff, 0xccb78fff, 0xa4a494ff, 0x8c846cff,    0x9c845cff, 0x9c7c54ff, 0x94744cff, 0x8c7454ff,
    0x846c54ff, 0x7b6747ff, 0x74644cff, 0x6c6454ff,    0xeacf09ff, 0xf0a705ff, 0xfe7f31ff, 0xfe5027ff,
    0xd10404ff, 0x9d1a1aff, 0x645c4cff, 0x6c5c44ff,    0x64543cff, 0x5c543cff, 0x545444ff, 0x4c5444ff,
    0x4c4c3cff, 0x544c3cff, 0x544c34ff, 0x5c4c34ff,    0x644c2cff, 0x64542cff, 0x6c5434ff, 0x745c34ff,
    0x7c542cff, 0x84542cff, 0x8c5c2cff, 0x8c5424ff,    0x9c6434ff, 0xa46c3cff, 0xb4743cff, 0xbc742cff,
    0xc47c34ff, 0xbc844cff, 0xbc8c54ff, 0xb48454ff,    0xac7c4cff, 0xcc8c4cff, 0xe88f37ff, 0xf49c54ff,
    0xcc7414ff, 0xdc6c04ff, 0xbc640cff, 0xac5c0cff,    0xac6c2cff, 0x8c6c44ff, 0x846444ff, 0x7c5c44ff,
    0x6c4c2cff, 0x644424ff, 0x5c442cff, 0x54442cff,    0x4c3c2cff, 0x443c2cff, 0x3c3c2cff, 0x3c3c34ff,
    0x343c34ff, 0x2c3434ff, 0x34342cff, 0x3d2e2eff,    0x3c2c24ff, 0x3c3424ff, 0x443424ff, 0x4c341cff,
    0x5c3c24ff, 0x643c1cff, 0x5c5a20ff, 0x444424ff,    0x444434ff, 0x24342cff, 0x242c2cff, 0x2c2424ff,
    0x2c2c1cff, 0x34241cff, 0x1c1c1cff, 0x14140cff,    0x0c0c0cff, 0x060605ff, 0x707522ff, 0x849324ff,
    0x94ac24ff, 0xa7c921ff, 0xb4dc24ff, 0xd4fc2cff,    0x041404ff, 0x0c1c04ff, 0x101104ff, 0x142c04ff,
    0x1c3404ff, 0x143414ff, 0x143c14ff, 0x144414ff,    0x144c14ff, 0x145414ff, 0x145c14ff, 0x0c4c0cff,
    0x0c440cff, 0x0c3c0cff, 0x04540cff, 0x2c4c0cff,    0x2c440cff, 0x344c0cff, 0x34540cff, 0x44640cff,
    0x4c740cff, 0x547c14ff, 0x548414ff, 0x5c9414ff,    0x649c14ff, 0x6cb41cff, 0x7cd41cff, 0x7c8444ff,
    0x6c6c3cff, 0x8c0000ff, 0x5b2b10ff, 0x201911ff,    0x2c1c14ff, 0x361f07ff, 0x3f2910ff, 0x463415ff,
    0xfbee9aff, 0xeecc81ff, 0xd3c7a5ff, 0xcfcab4ff,    0xc9baa3ff, 0xb7b099ff, 0xaca48cff, 0x9a9897ff,
    0x88949bff, 0x8c8c8cff, 0x7d848aff, 0x76716dff,    0x747c65ff, 0x766a57ff, 0x81693aff, 0x946c2cff,
    0x978872ff, 0x9f947aff, 0xbe9861ff, 0xeab456ff,    0xfc641cff, 0x666b72ff, 0xa4a4a4ff, 0xb4b4b4ff,
    0xb9bdc1ff, 0xccd0cfff, 0xe4d4c4ff, 0xc0321bff,    0x7d4e22ff, 0x605434ff, 0x595856ff, 0x2c1c2cff,
    0xff4155ff, 0xc7000eff, 0x88000dff, 0x68000aff,    0x500008ff, 0x380006ff, 0x44c1fcff, 0x0680c1ff,
    0x045b84ff, 0x044564ff, 0x04354cff, 0x042534ff,    0x41ff96ff, 0x00c760ff, 0x00883eff, 0x006830ff,
    0x005024ff, 0x003819ff, 0xffdd41ff, 0xc7aa00ff,    0x887100ff, 0x685600ff, 0x504200ff, 0x382e00ff,
    0xff8841ff, 0xc75100ff, 0x883400ff, 0x682800ff,    0x501e00ff, 0x381500ff, 0xff41c1ff, 0xc7007fff,
    0x88005aff, 0x680045ff, 0x500036ff, 0x380026ff,    0x4241ffff, 0x0700c7ff, 0x010088ff, 0x010068ff,
    0x000050ff, 0x000038ff, 0xa2a2a2ff, 0x616161ff,    0x3e3e3eff, 0x2b2b2bff, 0x1c1c1cff, 0x0b0b0bff,
    0xffffffff, 0xb1b1b1ff, 0x808080ff, 0x626262ff,    0x484848ff, 0x252525ff, 0x843c0cff, 0xd46414ff,
    0xfc9424ff, 0xfca424ff, 0xfcc434ff, 0xffe432ff,    0xfcfc3cff, 0xfcf46cff, 0xfcfc9cff, 0xe4c44cff,
    0x050583ff, 0x06066eff, 0x0202c4ff, 0x0202a0ff,    0x196c97ff, 0x115585ff, 0x10516dff, 0x1c80dcff,
    0x1770acff, 0x2476d1ff, 0x255698ff, 0x134572ff,    0x57350cff, 0x3e280dff, 0x31230cff, 0x040c3cff,
    0x0c1c64ff, 0x2c3cacff, 0x0c4cccff, 0x3c4cecff,    0x4c5ce4ff, 0x5c6cd4ff, 0x844cc4ff, 0x5414f4ff,
    0x1c84e4ff, 0x3474a4ff, 0x1c741cff, 0x1c9c1cff,    0x34d434ff, 0x44fc44ff, 0xfca4acff, 0xffffff00
};

class Image {

private:

    int width;
    int height;
    unsigned char *data;

public:

    Image(int width, int height) {
        this->width = width;
        this->height = height;
        this->data = new unsigned char[width * height];
    }

    Image(int width, int height, unsigned char *data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    unsigned char* getData() {
        return data;
    }

};

/**
 * Loads an image from a file.
 *
 * This is basically the reverse of `write_image` from image-extractor,
 * skipping all the data we don't care about.
 */
Image loadImage(std::string filename) {
    std::cout << "Loading: " << filename << "\n";

    std::ifstream ifs(filename, std::ios::binary | std::ios::in);
    if (!ifs) {
        throw std::runtime_error("Failed to load image!");
    }

    // Read sprite dimensions
    ifs.seekg(12);
    int width = ifs.get() | (ifs.get() << 8);
    int height = ifs.get() | (ifs.get() << 8);

    // Read pixel data
    unsigned char *data = new unsigned char[width * height];
    ifs.seekg(1042);
    ifs.read((char*) data, width * height);

    return Image(width, height, data);
}

/**
 * Writes an image to file.
 *
 * Based on `write_image` from image-extractor.
 */
int writeImage(std::string filename, Image& image) {

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

    fputc(0x00, fp); // ID Length
    fputc(0x01, fp); // Color map type
    fputc(0x01, fp); // Image type (uncompressed, colour-mapped)

    // Color map specification
    fputc(0, fp); // Index of first entry
    fputc(0x00, fp);
    fputc(0x00, fp);  // Number of entries (256)
    fputc(0x01, fp);
    fputc(32, fp);   // Entry size (32-bit RGBA)

    // Image specification
    fputc(0x00, fp); // X-origin
    fputc(0x00, fp);
    fputc(0x00, fp); // Y-origin
    fputc(0x00, fp);
    fputc((uint8_t) image.getWidth(), fp); // Width
    fputc((uint8_t)(image.getWidth() >> 8), fp);
    fputc((uint8_t) image.getHeight(), fp); // Height
    fputc((uint8_t)(image.getHeight() >> 8), fp);
    fputc(8, fp); // Bits per pixel

    // Image descriptor byte
    // (8 = number of alpha bits, bit5: upper-left origin)
    fputc(8 | 1 << 5, fp);

    // Colour map data
    for (int i = 0; i < PALETTE_SIZE; ++i) {

        const uint32_t col = PALETTE[i];
        const uint8_t red = (uint8_t)((col & 0xFF000000) >> 24);
        const uint8_t green = (uint8_t)((col & 0x00FF0000) >> 16);
        const uint8_t blue = (uint8_t)((col & 0x0000FF00) >> 8);
        const uint8_t alpha = (uint8_t)(col & 0x000000FF);

        fputc(blue, fp);
        fputc(green, fp);
        fputc(red, fp);
        fputc(alpha, fp);
    }

    // Pixel data
    unsigned char* data = image.getData();
    for (int y = 0; y < image.getHeight(); ++y) {
        for (int x = 0; x < image.getWidth(); ++x) {
            const uint8_t index = data[x + y * image.getWidth()];
            fputc(index, fp);
        }
    }

    fclose(fp);
    return 1;
}

/**
 * Copies pixels from one image into another.
 */
void copyImage(Image& src, Image& dst, int dstX, int dstY) {
    unsigned char* srcData = src.getData();
    unsigned char* dstData = dst.getData();

    for (int y = 0; y < src.getHeight(); y++) {
        for (int x = 0; x < src.getWidth(); x++) {

            int srcIndex = (y * src.getWidth()) + x;
            int dstIndex = ((dstY + y) * dst.getWidth()) + (dstX + x);

            dstData[dstIndex] = srcData[srcIndex];
        }
    }
}

/**
 * Rounds a number up to the nearest power of 2.
 *
 * See:
 * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
int nextPowerOf2(int v) {

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

int main() {

    // Load our definition file
    std::string filename = "scenery_meadow";
    std::ifstream file("definitions/" + filename + ".def");
    std::string line;

    int x = 0;
    int y = 0;

    int lastWidth = -1;
    int lastHeight = -1;

    std::vector<Image> sprites;

    // Load all sprites from the definition file
    while (std::getline(file, line)) {

        Image sprite = loadImage("images/" + line);

        // Check dimensions against the previous sprite
        if (lastWidth < 0) {
            lastWidth = sprite.getWidth();
            lastHeight = sprite.getHeight();
        }
        else if (lastWidth != sprite.getWidth()
            || lastHeight != sprite.getHeight()) {
            throw new std::runtime_error("Sprite dimensions do not match!");
        }

        sprites.push_back(sprite);
    }

    // Find the optimal texture size:
    // Start with a single long row of sprites, and keep splitting it until
    // we find a suitable size with minimal wasted space
    int tmpWidth = nextPowerOf2(lastWidth * sprites.size());
    int tmpHeight = lastHeight;
    int dataSize = tmpWidth * tmpHeight;
    int best = dataSize;
    int bestWidth = tmpWidth;
    int bestHeight = tmpHeight;

    while (tmpWidth > 256) {

        tmpWidth /= 2;
        tmpHeight *= 2;

        int area = tmpWidth * tmpHeight;
        int wastedSpace = area - dataSize;

        if (bestWidth > MAX_TEXTURE_SIZE || wastedSpace < best) {
            best = wastedSpace;
            bestWidth = tmpWidth;
            bestHeight = tmpHeight;
        }
    }

    std::cout << "Optimal size is " << bestWidth << " x " << bestHeight << "\n";

    if (bestWidth > MAX_TEXTURE_SIZE || bestHeight > MAX_TEXTURE_SIZE) {
        throw new std::runtime_error("Sprites will not fit!");
    }

    // Create an empty texture
    Image texture = Image(bestWidth, bestHeight);

    // Draw the sprites to the texture
    for (Image sprite : sprites) {
        std::cout << "Copying sprite to " << x << " x " << y << "\n";
        copyImage(sprite, texture, x, y);

        x += sprite.getWidth();

        if (x >= texture.getWidth()) {
            x = 0;
            y += sprite.getHeight();
        }
    }

    // Save the final texture
    writeImage("textures/" + filename + ".tga", texture);
}
