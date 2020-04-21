#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>
#include <string>

namespace Rival {

    class Texture {

    public:

        Texture(const GLuint id, int width, int height);

        //~Texture();

        const GLuint getId() const;

        const int getWidth() const;

        const int getHeight() const;

        static const Texture loadTexture(const std::string filename);

    private:

        const GLuint id;

        const int width;

        const int height;

    };

}

#endif // TEXTURE_H
