#ifndef TEXTURE_BUILDER_H
#define TEXTURE_BUILDER_H

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace TextureBuilder {

    ///////////////////////////////////////////////////////////////////////////////
    // Image class
    ///////////////////////////////////////////////////////////////////////////////

    class Image {

    public:
        Image(std::string filename, int width, int height);

        Image(std::string filename, int width, int height, const std::shared_ptr<unsigned char> data);

        std::string getFilename() const;

        int getWidth() const;

        int getHeight() const;

        std::shared_ptr<unsigned char> getData() const;

    private:
        std::string filename;
        int width;
        int height;
        std::shared_ptr<unsigned char> data;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Rect class
    ///////////////////////////////////////////////////////////////////////////////

    class Rect {

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

    class TextureAtlasBuilder {

    public:
        std::map<std::string, Image> imagesByKey;
        std::map<std::string, Rect> imagePlacements;
        std::vector<Rect> emptyRects;
        int texWidth;
        int texHeight;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // End of classes
    ///////////////////////////////////////////////////////////////////////////////

    bool createDirectory(const char* filename);

    void buildTextures(
            std::string definitionsDir,
            std::string imageDir,
            std::string outputDir,
            bool atlasMode);

    void readPalette(std::vector<std::uint32_t>& palette, const std::string filename);

    std::vector<Image> readImagesFromDefinitionFile(
            const std::string& imageDir,
            fs::path path,
            bool atlasMode);

    void createTextureAtlas(
            const std::string& imageDir,
            fs::path definitionFilename,
            std::vector<Image>& images,
            const std::vector<uint32_t>& palette);

    void createSpritesheetTexture(
            const std::string& imageDir,
            fs::path definitionFilename,
            const std::vector<Image>& sprites,
            const std::vector<uint32_t>& palette);

}  // namespace TextureBuilder

#endif  // TEXTURE_BUILDER_H
