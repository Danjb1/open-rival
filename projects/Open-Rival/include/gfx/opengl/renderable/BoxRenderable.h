#pragma once

#include "gfx/opengl/GLWrapper.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>

namespace Rival {

/** A solid-coloured rectangle. */
struct Box
{
    glm::vec2 topLeft;
    glm::vec2 bottomRight;

    /** RGBA, in the range 0-1. */
    glm::vec4 color;
};

/**
 * Class that allows one or more coloured quads to be rendered.
 *
 * This creates the VAO and all necessary buffers.
 */
class BoxRenderable
{
public:
    /**
     * Constructs a BoxRenderable.
     *
     * @param maxBoxes The maximum number of boxes that can be drawn.
     */
    BoxRenderable(int maxBoxes);

    /** Deletes a BoxRenderable. */
    ~BoxRenderable();

    // Disable moving / copying
    BoxRenderable(const BoxRenderable& other) = delete;
    BoxRenderable(BoxRenderable&& other) = delete;
    BoxRenderable& operator=(const BoxRenderable& other) = delete;
    BoxRenderable& operator=(BoxRenderable&& other) = delete;

    /** Uploads the given boxes to the GPU, replacing any previous ones.
     * Any boxes beyond the maximum are ignored. */
    void setBoxes(const std::vector<Box>& boxes);

    /** Draws the boxes most recently passed to setBoxes. */
    void draw() const;

private:
    static constexpr int numVertexDimensions = 2;  // x, y
    static constexpr int numColorDimensions = 4;   // r, g, b, a
    static constexpr int numVerticesPerBox = 4;

    GLuint vao;
    GLuint positionVbo;
    GLuint colorVbo;
    GLuint ibo;

    GLenum drawMode;

    int indicesPerBox;
    int maxBoxes;
    int numBoxes = 0;
};

}  // namespace Rival
