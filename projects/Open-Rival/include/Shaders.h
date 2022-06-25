#pragma once

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
    virtual std::string getName() const = 0;
    bool validateVertexAttribute(GLint attributeLoc, std::string attributeName) const;
    bool validateUniform(GLint uniformLoc, std::string uniformName) const;
};

///////////////////////////////////////////////////////////////////////////
// IndexedTextureShader:
// Renders an indexed texture, using an accompanying palette texture for
// color lookups.
///////////////////////////////////////////////////////////////////////////

class IndexedTextureShader : public Shader
{
public:
    GLuint programId;

    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc;

    // Vertex shader attribute locations
    GLint vertexAttribLoc;
    GLint texCoordAttribLoc;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc;
    GLint paletteTexUnitUniformLoc;

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
    GLuint programId;

    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc;

    // Vertex shader attribute locations
    GLint vertexAttribLoc;
    GLint texCoordAttribLoc;
    GLint colorAttribLoc;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc;

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
    GLuint programId;

    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc;

    // Vertex shader attribute locations
    GLint vertexAttribLoc;
    GLint texCoordAttribLoc;

    // Fragment shader uniform locations
    GLint texUnitUniformLoc;

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
