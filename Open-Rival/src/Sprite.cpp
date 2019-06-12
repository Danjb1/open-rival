#include "pch.h"
#include "Sprite.h"

namespace Rival {

    Sprite::Sprite(const Texture& texture, const int width, const int height) :
        texture(texture),
        width(width),
        height(height),
        xSize(texture.getWidth() / width),
        ySize(texture.getHeight() / height) {}

    std::vector<GLfloat> Sprite::getTexCoords(const int index) const {

        const int x = index % xSize;
        const int y = index / xSize;
        const float txWidth = static_cast<float>(texture.getWidth());
        const float txHeight = static_cast<float>(texture.getHeight());

        const float tx1 = x * width / txWidth;
        const float tx2 = (x + 1) * width / txWidth;

        // Subtract from 1 to account for the bottom-left OpenGL origin
        const float ty1 = 1 - ((y + 1) * height / txHeight);
        const float ty2 = 1 - (y * height / txHeight);

        return {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };
    }

}
