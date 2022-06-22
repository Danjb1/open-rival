#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

namespace Rival {

class TextRenderable;

class TextRenderer
{

public:
    /**
     * Renders a single TextRenderable.
     *
     * If the TextRenderable contains any unsupported characters, they will
     * be skipped.
     */
    void render(const TextRenderable& textRenderable) const;

private:
    GLuint vao;
    GLuint positionVbo;
    GLuint texCoordVbo;
    GLuint ibo;

    int indicesPerSprite;

    bool needsUpdate(const TextRenderable& textRenderable) const;
    void sendDataToGpu(const TextRenderable& textRenderable) const;
};

}  // namespace Rival
