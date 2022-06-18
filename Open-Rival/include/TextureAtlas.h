#pragma once

#include <gl/glew.h>

#include <unordered_map>
#include <vector>

#include "Rect.h"
#include "Texture.h"

namespace Rival {

class TextureAtlas
{

public:
    Texture texture;

    TextureAtlas(Texture texture, std::unordered_map<std::string, Rect> imagePlacements);

    const int getImageWidth(std::string key) const;
    const int getImageHeight(std::string key) const;

    const std::vector<GLfloat> getTexCoords(std::string key) const;

    static const TextureAtlas loadTextureAtlas(const std::string filename);

private:
    std::unordered_map<std::string, Rect> imagePlacements;
};

}  // namespace Rival
