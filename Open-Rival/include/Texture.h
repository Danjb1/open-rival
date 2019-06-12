#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>

namespace Rival {

    class Texture {

    private:

        const GLuint id;

        const int width;

        const int height;

    public:

        Texture(const GLuint id, const int width, const int height);

        GLuint getId() const;

        int getWidth() const;

        int getHeight() const;

    };

}

#endif // TEXTURE_H
