#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>

namespace Rival {

    class Texture {

    private:

        GLuint id;

        int width;

        int height;

    public:

        Texture(GLuint id, int width, int height);

        GLuint getId() const;

        int getWidth() const;

        int getHeight() const;

    };

}

#endif // TEXTURE_H
