#include "pch.h"

#include "TextureAtlas.h"

#include <iostream>

#include "BinaryFileReader.h"

namespace Rival {

TextureAtlas::TextureAtlas(Texture texture, std::unordered_map<std::string, Rect> imagePlacements)
    : texture(std::move(texture))
    , imagePlacements(imagePlacements)
{
}

const int TextureAtlas::getImageWidth(std::string key) const
{
    return static_cast<int>(imagePlacements.at(key).width);
}

const int TextureAtlas::getImageHeight(std::string key) const
{
    return static_cast<int>(imagePlacements.at(key).height);
}

const std::vector<GLfloat> TextureAtlas::getTexCoords(std::string key) const
{

    // Find the location of the image with the given key
    const Rect& imagePlacement = imagePlacements.at(key);

    // Calculate the image position in texture co-ordinates
    const float x = imagePlacement.x;
    const float y = imagePlacement.y;
    const float width = imagePlacement.width;
    const float height = imagePlacement.height;
    const float txWidth = static_cast<float>(texture.getWidth());
    const float txHeight = static_cast<float>(texture.getHeight());

    const float tx1 = x / txWidth;
    const float tx2 = (x + width) / txWidth;

    // Subtract from 1 to account for the bottom-left OpenGL origin
    const float ty1 = 1 - (y / txHeight);
    const float ty2 = 1 - ((y + height) / txHeight);

    return {
        tx1, ty1,  //
        tx2, ty1,  //
        tx2, ty2,  //
        tx1, ty2   //
    };
}

const TextureAtlas TextureAtlas::loadTextureAtlas(const std::string resourceName)
{

    std::string textureName = resourceName + ".tga";
    std::string atlasName = resourceName + ".atlas";
    std::unordered_map<std::string, Rect> imagePlacements;

    std::cout << "Loading: " << atlasName << "\n";

    std::ifstream reader(atlasName);
    if (!reader)
    {
        throw std::runtime_error("Failed to load atlas!");
    }

    while (reader.good())
    {
        std::string key;
        int x;
        int y;
        int width;
        int height;

        // Read the key
        reader >> key;

        // The last line of the file is just whitespace
        if (reader.eof())
        {
            break;
        }

        // Read the image position
        reader >> x >> y >> width >> height;

        // Add it to the map
        imagePlacements.emplace(key, Rect(x, y, width, height));
    }

    reader.close();

    return TextureAtlas(Texture::loadTexture(textureName), imagePlacements);
}

}  // namespace Rival
