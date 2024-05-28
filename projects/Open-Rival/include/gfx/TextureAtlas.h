#pragma once

#include "gfx/GlewWrapper.h"

#include <memory>
#include <unordered_map>
#include <vector>

#include "gfx/Texture.h"
#include "utils/Rect.h"

namespace Rival {

class TextureAtlas
{

public:
    TextureAtlas(std::shared_ptr<const Texture> texture, std::unordered_map<std::string, Rect> imagePlacements);

    const int getImageWidth(std::string key) const;
    const int getImageHeight(std::string key) const;

    const std::vector<GLfloat> getTexCoords(const std::string& key) const;

    static std::shared_ptr<const TextureAtlas> loadTextureAtlas(const std::string& filename);

public:
    std::shared_ptr<const Texture> texture;

private:
    std::unordered_map<std::string, Rect> imagePlacements;
};

}  // namespace Rival
