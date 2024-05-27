#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "gfx/Image.h"
#include "gfx/Palette.h"

namespace fs = std::filesystem;

namespace Rival { namespace Setup {

///////////////////////////////////////////////////////////////////////////////
// NamedImage struct
///////////////////////////////////////////////////////////////////////////////

struct NamedImage
{
    std::string name;
    Image image;

    // Wraps an Image by moving it into this NamedImage
    NamedImage(const std::string& name, Image&& image)
        : name(name)
        , image(std::move(image))
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
// Rect class
///////////////////////////////////////////////////////////////////////////////

struct Rect
{
    int x;
    int y;
    int width;
    int height;

    Rect(int x, int y, int width, int height);
};

///////////////////////////////////////////////////////////////////////////////
// TextureAtlasBuilder class
///////////////////////////////////////////////////////////////////////////////

class TextureAtlasBuilder
{
public:
    std::unordered_map<std::string, const NamedImage*> imagesByKey;
    std::unordered_map<std::string, Rect> imagePlacements;
    std::vector<Rect> emptyRects;
    int texWidth;
    int texHeight;

    TextureAtlasBuilder();

    void addImage(const NamedImage& img);

private:
    Rect findOrMakeEmptyRect(int reqWidth, int reqHeight);

    int findRectBiggerThan(const std::vector<Rect>& rects, int minWidth, int minHeight);

    int expandTextureToFitRect(int reqWidth, int reqHeight);
};

///////////////////////////////////////////////////////////////////////////////
// End of classes
///////////////////////////////////////////////////////////////////////////////

void buildTextures(const std::string& definitionsDir, const std::string& imageDir, const std::string& outputDir);

void readPalette(Palette::Palette& outPalette, const std::string& filename);

std::vector<NamedImage> readDefinitionFile(
        const std::string& imageDir, const fs::path& path, bool& outAtlasMode, int& outPadding);

void createTextureAtlas(const std::string& imageDir,
        const fs::path& definitionFilename,
        std::vector<NamedImage>& images,
        const Palette::Palette& palette);

void createSpritesheetTexture(const std::string& imageDir,
        const fs::path& definitionFilename,
        const std::vector<NamedImage>& sprites,
        const Palette::Palette& palette,
        int padding);

}}  // namespace Rival::Setup
