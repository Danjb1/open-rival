#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Image.h"
#include "Palette.h"

namespace fs = std::filesystem;

namespace Rival { namespace Setup {

///////////////////////////////////////////////////////////////////////////////
// NamedImage class
///////////////////////////////////////////////////////////////////////////////

class NamedImage
{
public:
    std::string name;
    Image image;

    // Wraps an Image by moving it into this NamedImage
    NamedImage(const std::string name, Image&& image)
        : name(name)
        , image(std::move(image))
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
// Rect class
///////////////////////////////////////////////////////////////////////////////

class Rect
{

public:
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
    Rect findOrMakeEmptyRect(const int reqWidth, const int reqHeight);

    int findRectBiggerThan(const std::vector<Rect>& rects, const int minWidth, const int minHeight);

    int expandTextureToFitRect(const int reqWidth, const int reqHeight);
};

///////////////////////////////////////////////////////////////////////////////
// End of classes
///////////////////////////////////////////////////////////////////////////////

void buildTextures(std::string definitionsDir, std::string imageDir, std::string outputDir);

void readPalette(Palette::Palette& palette, const std::string filename);

std::vector<NamedImage>
readDefinitionFile(const std::string& imageDir, const fs::path& path, bool& outAtlasMode, int& outPadding);

void createTextureAtlas(
        const std::string& imageDir,
        fs::path definitionFilename,
        std::vector<NamedImage>& images,
        const Palette::Palette& palette);

void createSpritesheetTexture(
        const std::string& imageDir,
        fs::path definitionFilename,
        const std::vector<NamedImage>& sprites,
        const Palette::Palette& palette,
        int padding);

}}  // namespace Rival::Setup
