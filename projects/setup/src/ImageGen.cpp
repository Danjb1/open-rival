#include <windows.h>  // CreateDirectoryA, etc.

#include <cstdint>
#include <iostream>
#include <vector>  // vector

#include "gfx/Palette.h"
#include "setup-utils.h"

namespace Rival { namespace Setup {

/**
 * Makes all the images needed for the health overlay.
 *
 * We could try to render health bars as a gradient using coloured quads but it is much easier to get the colors right
 * if we use a texture.
 */
void makeHealthOverlays(const std::string& outputDir)
{
    const int width = 1;
    const int height = 3;

    // Unit health bar (full)
    Image image = Image::createEmpty(width, height, 0xff);
    image.setPixel(0, 0, 172);
    image.setPixel(0, 1, 174);
    image.setPixel(0, 2, 176);
    std::string filename = outputDir + "\\img_overlay_health.tga";
    std::cout << "Saving image " << filename << "\n";
    writeImage(image, Palette::paletteGame, filename);

    // Unit health bar (depleted)
    image = Image::createEmpty(width, height, 0xff);
    image.setPixel(0, 0, 160);
    image.setPixel(0, 1, 162);
    image.setPixel(0, 2, 164);
    filename = outputDir + "\\img_overlay_health_depleted.tga";
    std::cout << "Saving image " << filename << "\n";
    writeImage(image, Palette::paletteGame, filename);

    // Monster health bar (full)
    image = Image::createEmpty(width, height, 0xff);
    image.setPixel(0, 0, 255);
    image.setPixel(0, 1, 210);
    image.setPixel(0, 2, 212);
    filename = outputDir + "\\img_overlay_health_monster.tga";
    std::cout << "Saving image " << filename << "\n";
    writeImage(image, Palette::paletteGame, filename);

    // Monster health bar (depleted)
    image = Image::createEmpty(width, height, 0xff);
    image.setPixel(0, 0, 190);
    image.setPixel(0, 1, 192);
    image.setPixel(0, 2, 194);
    filename = outputDir + "\\img_overlay_health_monster_depleted.tga";
    std::cout << "Saving image " << filename << "\n";
    writeImage(image, Palette::paletteGame, filename);
}

void makeProceduralImages(const std::string& outputDir)
{
    makeHealthOverlays(outputDir);
}

}}  // namespace Rival::Setup
