#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <gl/glew.h>
#include <map>
#include <vector>

#include "Rect.h"
#include "Texture.h"

namespace Rival {

    class TextureAtlas {

    public:
        Texture texture;

        TextureAtlas(
                Texture texture,
                std::map<std::string, Rect> imagePlacements);

        const int TextureAtlas::getImageWidth(std::string key) const;
        const int TextureAtlas::getImageHeight(std::string key) const;

        const std::vector<GLfloat> getTexCoords(std::string key) const;

        static const TextureAtlas TextureAtlas::loadTextureAtlas(
                const std::string filename);

    private:
        std::map<std::string, Rect> imagePlacements;
    };

}  // namespace Rival

#endif  // TEXTURE_ATLAS_H
