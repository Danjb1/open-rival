#include "setup-utils.h"

#include <windows.h>  // CreateDirectoryA, etc.

#include <cstdint>   // uint8_t
#include <fstream>   // ofstream
#include <iomanip>   // setw, setfill
#include <sstream>   // ostringstream
#include <vector>    // vector

#include "gfx/Palette.h"

namespace Rival { namespace Setup {

/**
 * Attempts to create the given directory.
 */
bool createDirectory(const std::string dirName)
{
    return CreateDirectoryA(dirName.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
}

/**
 * Writes an image to disk.
 *
 * Our images are saved using a top-left origin, which means that the first byte of the data on disk is the top-left
 * corner of the image. Conveniently, this means that when we send the data to OpenGL (which expects the first byte to
 * be the bottom-left corner), the image gets flipped to suit the OpenGL origin. This means that we don't need to do any
 * funky transformations when generating texture co-ordinates; we can still use (0, 0) to refer to the top-left corner
 * of the image (since OpenGL will be looking at the bottom-left corner of an upside-down image).
 */
void writeImage(Image& image, const Palette::Palette& palette, const std::string filename)
{
    // Open file for writing
    std::ofstream out;
    out.open(filename, std::ios::out | std::ios::binary);

    /*
     * Write using TGA format:
     * http://tfc.duke.free.fr/coding/tga_specs.pdf
     */

    out.put(0x00);  // ID Length
    out.put(0x01);  // Color map type
    out.put(0x01);  // Image type (uncompressed, color-mapped)

    // Color map specification
    out.put(0);  // Index of first entry
    out.put(0x00);
    out.put(0x00);  // Number of entries (256)
    out.put(0x01);
    out.put(32);  // Entry size (32-bit RGBA)

    // Image specification
    out.put(0x00);  // X-origin
    out.put(0x00);
    out.put(0x00);  // Y-origin
    out.put(0x00);
    int w = image.getWidth();
    int h = image.getHeight();
    out.put((uint8_t) w);  // Width
    out.put((uint8_t) (w >> 8));
    out.put((uint8_t) h);  // Height
    out.put((uint8_t) (h >> 8));
    out.put(8);  // Bits per pixel

    // Image descriptor byte
    // (8 = number of alpha bits, bit5: upper-left origin)
    out.put(8 | 1 << 5);

    // Color map data
    for (int i = 0; i < Palette::numColors; ++i)
    {
        const std::uint32_t col = palette[i];

        const std::uint8_t red = (std::uint8_t)((col & 0xFF000000) >> 24);
        const std::uint8_t green = (std::uint8_t)((col & 0x00FF0000) >> 16);
        const std::uint8_t blue = (std::uint8_t)((col & 0x0000FF00) >> 8);
        const std::uint8_t alpha = (std::uint8_t)(col & 0x000000FF);

        out.put(static_cast<char>(blue));
        out.put(static_cast<char>(green));
        out.put(static_cast<char>(red));
        out.put(static_cast<char>(alpha));
    }

    // Pixel data
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            out.put(image.getPixel(x, y));
        }
    }
}

std::string zeroPad(int value, int numDigits)
{
    std::ostringstream oss;
    oss << std::setw(numDigits) << std::setfill('0') << value;
    return oss.str();
}

}}  // namespace Rival::Setup
