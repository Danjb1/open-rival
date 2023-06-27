#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include <string>

namespace Rival { namespace Shaders {

// Vertex shader attribute indices.
// We keep these common between all shaders so that our VAOs can be
// rendered by any shader without requiring reconfiguration.
static constexpr GLint vertexAttribIndex = 0;
static constexpr GLint texCoordAttribIndex = 1;
static constexpr GLint colorAttribIndex = 2;

///////////////////////////////////////////////////////////////////////////
// Shader base class
///////////////////////////////////////////////////////////////////////////

class Shader
{
public:
    GLuint programId = 0;

    virtual std::string getName() const = 0;
    bool validateVertexAttribute(GLint attributeLoc, const std::string& attributeName) const;
    bool validateUniform(GLint uniformLoc, const std::string& uniformName) const;
};

///////////////////////////////////////////////////////////////////////////
// IndexedTextureShader:
// Renders an indexed texture, using an accompanying palette texture for
// color lookups.
///////////////////////////////////////////////////////////////////////////

class IndexedTextureShader : public Shader
{
public:
    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc = -1;

    // Vertex shader attribute locations
    GLint vertexAttribLoc = -1;
    GLint texCoordAttribLoc = -1;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc = -1;
    GLint paletteTexUnitUniformLoc = -1;
    GLint paletteTxYUnitUniformLoc = -1;
    GLint transparentIndex = -1;

    static void init();

    bool isValid() const;

    std::string getName() const override
    {
        return "IndexedTextureShader";
    }
};

extern IndexedTextureShader indexedTextureShader;

///////////////////////////////////////////////////////////////////////////
// FontShader:
// Renders a single-channel font texture in a custom color.
///////////////////////////////////////////////////////////////////////////

class FontShader : public Shader
{
public:
    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc = -1;

    // Vertex shader attribute locations
    GLint vertexAttribLoc = -1;
    GLint texCoordAttribLoc = -1;
    GLint colorAttribLoc = -1;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc = -1;

    static void init();

    bool isValid() const;

    std::string getName() const override
    {
        return "FontShader";
    }
};

extern FontShader fontShader;

///////////////////////////////////////////////////////////////////////////
// TextureShader:
// Renders a texture in 2D space (no depth).
///////////////////////////////////////////////////////////////////////////

class TextureShader : public Shader
{
public:
    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc = -1;

    // Vertex shader attribute locations
    GLint vertexAttribLoc = -1;
    GLint texCoordAttribLoc = -1;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc = -1;

    static void init();

    bool isValid() const;

    std::string getName() const override
    {
        return "TextureShader";
    }
};

extern TextureShader textureShader;

///////////////////////////////////////////////////////////////////////////
// Generic methods
///////////////////////////////////////////////////////////////////////////

void initializeShaders();

GLuint createShader(const char* vertShader, const char* fragShader);

}}  // namespace Rival::Shaders
