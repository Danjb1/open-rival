#include <windows.h>  // CreateDirectoryA, etc.

#include <cstdint>
#include <iostream>
#include <vector>  // vector

#include "gfx/Palette.h"
#include "setup-utils.h"

namespace Rival { namespace Setup {

/**
 * Makes an image containing all the bars needed for the health overlay.
 *
 * We could try to render health bars as a gradient using coloured quads but it is much easier to get the colors right
 * if we use a texture.
 */
void makeHealthOverlay(const std::string& outputDir)
{
    // N.B. Use power-of-two dimensions!
    const int width = 8;
    const int height = 8;
    Image image = Image::createEmpty(width, height, 0xff);

    // Unit health bar (full)
    image.setPixel(0, 0, 172);
    image.setPixel(0, 1, 174);
    image.setPixel(0, 2, 176);

    // Unit health bar (depleted)
    image.setPixel(1, 0, 160);
    image.setPixel(1, 1, 162);
    image.setPixel(1, 2, 164);

    // Monster health bar (full)
    image.setPixel(2, 0, 255);
    image.setPixel(2, 1, 210);
    image.setPixel(2, 2, 212);

    // Monster health bar (depleted)
    image.setPixel(3, 0, 190);
    image.setPixel(3, 1, 192);
    image.setPixel(3, 2, 194);

    // Save image to disk
    const std::string filename = outputDir + "\\img_overlay_health.tga";
    std::cout << "Saving image " << filename << "\n";
    writeImage(image, Palette::paletteGame, filename);
}

void makeProceduralImages(const std::string& outputDir)
{
    makeHealthOverlay(outputDir);
}

}}  // namespace Rival::Setup
