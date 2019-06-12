#include "pch.h"
#include "Sprite.h"

namespace Rival {

    Sprite::Sprite(const Texture& texture, const int width, const int height) :
        texture(texture),
        width(width),
        height(height) {}

    std::vector<GLfloat> Sprite::getTexCoords(const int index) const {
        std::vector<GLfloat> texCoords = {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        };
        return texCoords;
    }

}
