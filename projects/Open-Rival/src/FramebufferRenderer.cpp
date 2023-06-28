#include "FramebufferRenderer.h"

#define GLEW_STATIC
#include <gl/glew.h>

#include <vector>

#include "Framebuffer.h"
#include "RenderUtils.h"
#include "Shaders.h"

namespace Rival {

const glm::mat4 FramebufferRenderer::viewProjectionMatrix = createViewProjectionMatrix();

const glm::mat4 FramebufferRenderer::createViewProjectionMatrix()
{
    // Our view-projection matrix is always the same because our framebuffers use identical vertices
    glm::mat4 view = RenderUtils::createViewMatrix(0.5f, 0.5f);
    glm::mat4 projection = RenderUtils::createProjectionMatrix(1.f, 1.f);
    return projection * view;
}

FramebufferRenderer::FramebufferRenderer(Framebuffer& fbo)
    : fbo(fbo)
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &texCoordVbo);
    glGenBuffers(1, &ibo);

    // Define a basic quad from (0,0) to (1,1)
    const float x1 = 0.0f;
    const float y1 = 0.0f;
    const float x2 = 1.0f;
    const float y2 = 1.0f;
    std::vector<GLfloat> vertexData = {
        x1, y1,  //
        x2, y1,  //
        x2, y2,  //
        x1, y2   //
    };

    // Initialize position buffer - this should never change
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    // Initialize tex co-ord buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glVertexAttribPointer(
            Shaders::texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);
    int texCoordBufferSize = numTexCoordDimensions * numIndices * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData = { 0, 1, 2, 3 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
}

void FramebufferRenderer::render(int srcWidth, int srcHeight) const
{
    // Bind vertex array
    glBindVertexArray(vao);

    // Use the framebuffer's texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.getTextureId());

    // Define the portion of the texture to be sampled.
    // We flip ty1 and ty2 because OpenGL expects the texture origin to be in the bottom-left, but we render to the
    // framebuffer using a top-elft origin.
    const float tx1 = 0.f;
    const float ty1 = static_cast<float>(srcHeight) / fbo.getHeight();
    const float tx2 = static_cast<float>(srcWidth) / fbo.getWidth();
    const float ty2 = 0.f;
    std::vector<GLfloat> texCoords = {
        tx1, ty1,  //
        tx2, ty1,  //
        tx2, ty2,  //
        tx1, ty2   //
    };

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(GLfloat), texCoords.data());

    // Render
    glDrawElements(GL_TRIANGLE_FAN, numIndices, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Rival
