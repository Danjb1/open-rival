#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>
#include <string>

#include "Image.h"

namespace Rival {

    class Texture {

    public:
        Texture(const GLuint id, int width, int height);

        const GLuint getId() const;

        const int getWidth() const;

        const int getHeight() const;

        static const Texture loadTexture(const std::string filename);

        static const Texture wrap(const Image img);

    private:
        const GLuint id;

        const int width;

        const int height;
    };

}  // namespace Rival

#endif  // TEXTURE_H
