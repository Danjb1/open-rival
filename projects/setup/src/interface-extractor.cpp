/**
 * Script to extract the Rival Realms images from "Interfac.dat".
 */

#include "pch.h"

#include "interface-extractor.h"

#include <stdexcept>

#include "setup-utils.h"

namespace Rival { namespace Setup {

// NOTE: It is possible these offsets will differ for other locales.
// In this case, maybe we can just "skip 'n' bytes" instead of jumping to
// specific offsets.
const int offsetTitle = 0x2131;
const int offsetInGameMenu = 0x1685AE4;
const int offsetPostgameMenu = 0x16F85A9;
const int offsetCursors = 0x1932F0A;
const int offsetHireTroops = 0x1938DBB;

InterfaceExtractor::InterfaceExtractor(std::string inputFile)
    : index(0)
    , reader(inputFile)
{
}

void InterfaceExtractor::extractImages(std::string outputDir)
{
    // I am not sure what comes before this offset but I am fairly confident
    // there are no more images. Possibly the palettes or image offsets are
    // included in this file.
    reader.setPos(offsetTitle);
    int index = 0;

    // Title image
    extractImage(outputDir, Palette::paletteTitle);

    // Menu backgrounds
    for (int i = 0; i < 9; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }

    // "Hire Troops" background
    extractImage(outputDir, Palette::paletteHireTroops);

    // Campaign backgrounds
    for (int i = 0; i < 3; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }

    // Loading screen backgrounds
    for (int i = 0; i < 2; i++)
    {
        extractImage(outputDir, Palette::paletteLoading);
    }

    // Campaign level intros and menu components
    for (int i = 0; i < 128; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }

    // "Hire Troops" menu components
    for (int i = 0; i < 20; i++)
    {
        extractImage(outputDir, Palette::paletteHireTroops);
    }

    // Menu components continued
    for (int i = 0; i < 61; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }
    for (int i = 0; i < 2; i++)
    {
        extractImage(outputDir, Palette::paletteHireTroops);
    }
    for (int i = 0; i < 2; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }

    // Loading screen components
    for (int i = 0; i < 14; i++)
    {
        extractImage(outputDir, Palette::paletteLoading);
    }

    // In-game menu
    reader.setPos(offsetInGameMenu);
    for (int i = 0; i < 75; i++)
    {
        extractImage(outputDir, Palette::paletteGame);
    }

    // Postgame menu components
    reader.setPos(offsetPostgameMenu);
    for (int i = 0; i < 41; i++)
    {
        extractImage(outputDir, Palette::paletteMenu);
    }

    // "Save Troops" menu components
    for (int i = 0; i < 25; i++)
    {
        extractImage(outputDir, Palette::paletteHireTroops);
    }

    // Cursors
    // TODO: We skip a lot of bytes in this section. These may be related to
    // the number of frames in an animated cursor, or possibly they are just
    // there to align the data somehow. Without deciphering this reason, it is
    // not feasible to wrap these in a single loop.
    reader.setPos(offsetCursors);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    reader.skip(2);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);
    extractImage(outputDir, Palette::paletteGame);

    // Game icons
    reader.setPos(offsetHireTroops);
    for (int i = 0; i < 831; i++)
    {
        extractImage(outputDir, Palette::paletteGame);
    }
}

void InterfaceExtractor::extractImage(const std::string& outputDir, const Palette::Palette& palette)
{
    // Read dimensions
    std::uint16_t w = reader.readShort();
    std::uint16_t h = reader.readShort();
    int imageSize = w * h;
    std::vector<std::uint8_t> pixels(imageSize);

    if (w > 800 || h > 600)
    {
        throw std::runtime_error("Invalid image dimensions");
    }

    // 4 unknown bytes
    reader.skip(4);

    // Read pixels
    int nextPxIndex = 0;
    while (nextPxIndex < imageSize)
    {

        int next = reader.readByte();

        if (next > 0x80)
        {
            // Next 'n' pixels are all the same
            int numSamePixels = next - 0x80;
            int paletteIndex = reader.readByte();
            for (int i = 0; i < numSamePixels; i++)
            {
                if (nextPxIndex >= imageSize)
                {
                    throw std::runtime_error("Invalid image format");
                }
                pixels[nextPxIndex] = paletteIndex;
                nextPxIndex++;
            }
        }
        else
        {
            // Next 'n' pixels are all different
            int numDifferentPixels = next;
            for (int i = 0; i < numDifferentPixels; i++)
            {
                if (nextPxIndex >= imageSize)
                {
                    throw std::runtime_error("Invalid image format");
                }
                int paletteIndex = reader.readByte();
                if (paletteIndex >= Palette::numColors)
                {
                    throw std::runtime_error("Invalid palette reference");
                }
                pixels[nextPxIndex] = paletteIndex;
                nextPxIndex++;
            }
        }
    }

    // Generate the image filename
    std::string filename = outputDir + "\\img_ui_" + zeroPad(index, 4) + ".tga";

    // Save the rendered image to disk
    Image image = Image::createByMove(static_cast<int>(w), static_cast<int>(h), std::move(pixels));
    writeImage(image, palette, filename);

    // Skip some unknown bytes
    // TODO: Trying to read these "skipped" bytes causes the program to crash
    //   after the last image, which leads me to believe these bytes are
    //   probably supposed to come BEFORE each image, not after it
    std::uint16_t next = reader.readShort();
    if (next != 0x03)
    {
        reader.skip(2);
    }

    ++index;
}

}}  // namespace Rival::Setup
