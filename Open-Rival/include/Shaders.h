#ifndef SHADERS_H
#define SHADERS_H

#include <gl/glew.h>

#include <string>

namespace Rival { namespace Shaders {

    // Vertex shader attribute indices.
    // We keep these common between all shaders so that our VAOs can be
    // rendered by any shader without requiring reconfiguration.
    static const GLint vertexAttribIndex = 0;
    static const GLint texCoordAttribIndex = 1;
    static const GLint colorAttribIndex = 2;

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
    // Renders a texture using a view-projection matrix and an accompanying
    // palette texture for color lookups.
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
    // Renders a single-channel texture as an alpha channel, using a
    // view-projection matrix and a color buffer.
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
    // ScreenShader:
    // Just renders a texture without applying any transformations.
    ///////////////////////////////////////////////////////////////////////////

    class ScreenShader : public Shader
    {
    public:
        GLuint programId;

        // Vertex shader attribute locations
        GLint vertexAttribLoc;
        GLint texCoordAttribLoc;

        // Fragment shader uniform locations
        GLint texUnitUniformLoc;

        static void init();

        bool isValid() const;

        std::string getName() const override
        {
            return "ScreenShader";
        }
    };

    extern ScreenShader screenShader;

    ///////////////////////////////////////////////////////////////////////////
    // Generic methods
    ///////////////////////////////////////////////////////////////////////////

    void initializeShaders();

    GLuint createShader(const char* vertShader, const char* fragShader);

}}  // namespace Rival::Shaders

#endif  // SHADERS_H
