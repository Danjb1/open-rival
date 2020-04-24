#include "pch.h"
#include "FramebufferRenderer.h"

#include <gl/glew.h>
#include <vector>

#include "Shaders.h"

namespace Rival {

    FramebufferRenderer::FramebufferRenderer(Framebuffer& fbo) :
        fbo(fbo) {}

    void FramebufferRenderer::init() {

        // Generate VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate VBOs
        glGenBuffers(1, &positionVbo);
        glGenBuffers(1, &texCoordVbo);
        glGenBuffers(1, &ibo);

        // Define a quad that fills the entire viewport
        float x1 = -1.0f;
        float y1 = -1.0f;
        float x2 = 1.0f;
        float y2 = 1.0f;
        std::vector<GLfloat> vertexData = {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };

        // Initialise position buffer
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glVertexAttribPointer(
                screenShader.vertexAttribIndex,
                numVertexDimensions,
                GL_FLOAT,
                GL_FALSE,
                numVertexDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(
                GL_ARRAY_BUFFER,
                vertexData.size() * sizeof(GLfloat),
                vertexData.data(),
                GL_STATIC_DRAW);

        // Define tex co-ords to sample the entire texture
        float tx1 = 0.0f;
        float ty1 = 0.0f;
        float tx2 = 1.0f;
        float ty2 = 1.0f;
        std::vector<GLfloat> texCoords = {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };

        // Initialise tex co-ord buffer
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glVertexAttribPointer(
                screenShader.texCoordAttribIndex,
                numTexCoordDimensions,
                GL_FLOAT,
                GL_FALSE,
                numTexCoordDimensions * sizeof(GLfloat),
                nullptr);
        glBufferData(
                GL_ARRAY_BUFFER,
                texCoords.size() * sizeof(GLfloat),
                texCoords.data(),
                GL_STATIC_DRAW);

        // Initialise index buffer - this should never need to change
        std::vector<GLuint> indexData = { 3, 2, 1, 0 };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                indexData.size() * sizeof(GLuint),
                indexData.data(),
                GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(screenShader.vertexAttribIndex);
        glEnableVertexAttribArray(screenShader.texCoordAttribIndex);
    }

    void FramebufferRenderer::render() {

        // Bind vertex array
        glBindVertexArray(vao);

        // Use the framebuffer's texture
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, fbo.getTextureId());

        // Render
        glDrawElements(
            GL_TRIANGLE_FAN,
            numIndices,
            GL_UNSIGNED_INT,
            nullptr);
    }


}
