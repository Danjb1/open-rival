#include "gfx/Shaders.h"

#include <iostream>
#include <stdexcept>

#include "gfx/ShaderUtils.h"

namespace Rival { namespace Shaders {

///////////////////////////////////////////////////////////////////////////
// Shader
///////////////////////////////////////////////////////////////////////////

bool Shader::validateVertexAttribute(GLint attributeLoc, const std::string& attributeName) const
{
    if (attributeLoc == -1)
    {
        std::cout << "Could not locate vertex attribute " << attributeName << " for shader " << getName() << "\n";
        return false;
    }
    return true;
}

bool Shader::validateUniform(GLint uniformLoc, const std::string& uniformName) const
{
    if (uniformLoc == -1)
    {
        std::cout << "Could not locate uniform " << uniformName << " for shader " << getName() << "\n";
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
// IndexedTextureShader
///////////////////////////////////////////////////////////////////////////

IndexedTextureShader indexedTextureShader;

void IndexedTextureShader::init()
{

    GLuint programId = createShader("res\\shaders\\indexed_texture.vert", "res\\shaders\\indexed_texture.frag");

    indexedTextureShader.programId = programId;
    indexedTextureShader.vertexAttribLoc = glGetAttribLocation(programId, "in_vertex");
    indexedTextureShader.texCoordAttribLoc = glGetAttribLocation(programId, "in_tex_coords");
    indexedTextureShader.viewProjMatrixUniformLoc = glGetUniformLocation(programId, "view_proj_matrix");
    indexedTextureShader.texUnitUniformLoc = glGetUniformLocation(programId, "tex");
    indexedTextureShader.paletteTexUnitUniformLoc = glGetUniformLocation(programId, "palette");
    indexedTextureShader.paletteTxYUnitUniformLoc = glGetUniformLocation(programId, "palette_txy");
    indexedTextureShader.transparentIndex = glGetUniformLocation(programId, "transparent_index");

    if (!indexedTextureShader.isValid())
    {
        throw std::runtime_error("Failed to create IndexedTextureShader");
    }
}

bool IndexedTextureShader::isValid() const
{
    // Validate program ID
    if (programId == 0)
    {
        printf("Could not generate program ID\n");
        return false;
    }

    // Validate vertex attributes / uniforms
    return validateVertexAttribute(vertexAttribLoc, "in_vertex")              //
            && validateVertexAttribute(texCoordAttribLoc, "in_tex_coords")    //
            && validateUniform(viewProjMatrixUniformLoc, "view_proj_matrix")  //
            && validateUniform(texUnitUniformLoc, "tex")                      //
            && validateUniform(paletteTexUnitUniformLoc, "palette")           //
            && validateUniform(paletteTexUnitUniformLoc, "palette_txy")       //
            && validateUniform(transparentIndex, "transparent_index");
}

///////////////////////////////////////////////////////////////////////////
// WorldShader
///////////////////////////////////////////////////////////////////////////

WorldShader worldShader;

void WorldShader::init()
{

    GLuint programId = createShader("res\\shaders\\world.vert", "res\\shaders\\world.frag");

    worldShader.programId = programId;
    worldShader.vertexAttribLoc = glGetAttribLocation(programId, "in_vertex");
    worldShader.texCoordAttribLoc = glGetAttribLocation(programId, "in_tex_coords");
    worldShader.viewProjMatrixUniformLoc = glGetUniformLocation(programId, "view_proj_matrix");
    worldShader.texUnitUniformLoc = glGetUniformLocation(programId, "tex");
    worldShader.paletteTexUnitUniformLoc = glGetUniformLocation(programId, "palette");
    worldShader.paletteTxYUnitUniformLoc = glGetUniformLocation(programId, "palette_txy");
    worldShader.transparentIndex = glGetUniformLocation(programId, "transparent_index");
    worldShader.waterShift1 = glGetUniformLocation(programId, "water_shift_1");
    worldShader.waterShift2 = glGetUniformLocation(programId, "water_shift_2");

    if (!worldShader.isValid())
    {
        throw std::runtime_error("Failed to create WorldShader");
    }
}

bool WorldShader::isValid() const
{
    // Validate program ID
    if (programId == 0)
    {
        printf("Could not generate program ID\n");
        return false;
    }

    // Validate vertex attributes / uniforms
    return validateVertexAttribute(vertexAttribLoc, "in_vertex")              //
            && validateVertexAttribute(texCoordAttribLoc, "in_tex_coords")    //
            && validateUniform(viewProjMatrixUniformLoc, "view_proj_matrix")  //
            && validateUniform(texUnitUniformLoc, "tex")                      //
            && validateUniform(paletteTexUnitUniformLoc, "palette")           //
            && validateUniform(paletteTexUnitUniformLoc, "palette_txy")       //
            && validateUniform(transparentIndex, "transparent_index")         //
            && validateUniform(waterShift1, "water_shift_1")                  //
            && validateUniform(waterShift2, "water_shift_2");
}

///////////////////////////////////////////////////////////////////////////
// FontShader
///////////////////////////////////////////////////////////////////////////

FontShader fontShader;

void FontShader::init()
{

    GLuint programId = createShader("res\\shaders\\font.vert", "res\\shaders\\font.frag");

    fontShader.programId = programId;
    fontShader.vertexAttribLoc = glGetAttribLocation(programId, "in_vertex");
    fontShader.texCoordAttribLoc = glGetAttribLocation(programId, "in_tex_coords");
    fontShader.colorAttribLoc = glGetAttribLocation(programId, "in_color");
    fontShader.viewProjMatrixUniformLoc = glGetUniformLocation(programId, "view_proj_matrix");
    fontShader.texUnitUniformLoc = glGetUniformLocation(programId, "tex");

    if (!fontShader.isValid())
    {
        throw std::runtime_error("Failed to create FontShader");
    }
}

bool FontShader::isValid() const
{
    // Validate program ID
    if (programId == 0)
    {
        printf("Could not generate program ID\n");
        return false;
    }

    // Validate vertex attributes / uniforms
    return validateVertexAttribute(vertexAttribLoc, "in_vertex")              //
            && validateVertexAttribute(texCoordAttribLoc, "in_tex_coords")    //
            && validateVertexAttribute(colorAttribLoc, "in_color")            //
            && validateUniform(viewProjMatrixUniformLoc, "view_proj_matrix")  //
            && validateUniform(texUnitUniformLoc, "tex");
}

///////////////////////////////////////////////////////////////////////////
// TextureShader
///////////////////////////////////////////////////////////////////////////

TextureShader textureShader;

void TextureShader::init()
{

    GLuint programId = createShader("res\\shaders\\texture.vert", "res\\shaders\\texture.frag");

    textureShader.programId = programId;
    textureShader.vertexAttribLoc = glGetAttribLocation(programId, "in_vertex");
    textureShader.texCoordAttribLoc = glGetAttribLocation(programId, "in_tex_coords");
    textureShader.viewProjMatrixUniformLoc = glGetUniformLocation(programId, "view_proj_matrix");
    textureShader.texUnitUniformLoc = glGetUniformLocation(programId, "tex");

    if (!textureShader.isValid())
    {
        throw std::runtime_error("Failed to create TextureShader");
    }
}

bool TextureShader::isValid() const
{
    // Validate program ID
    if (programId == 0)
    {
        printf("Could not generate program ID\n");
        return false;
    }

    // Validate vertex attributes / uniforms
    return validateVertexAttribute(vertexAttribLoc, "in_vertex")              //
            && validateVertexAttribute(texCoordAttribLoc, "in_tex_coords")    //
            && validateUniform(viewProjMatrixUniformLoc, "view_proj_matrix")  //
            && validateUniform(texUnitUniformLoc, "tex");
}

///////////////////////////////////////////////////////////////////////////
// Generic methods
///////////////////////////////////////////////////////////////////////////

void initializeShaders()
{
    IndexedTextureShader::init();
    WorldShader::init();
    FontShader::init();
    TextureShader::init();
}

GLuint createShader(const char* vertShader, const char* fragShader)
{

    GLuint programId = glCreateProgram();

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Read / set vertex source
    std::string vertexShaderSource = ShaderUtils::readShaderSource(vertShader);
    const char* vertexShaderSource2 = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource2, nullptr);

    // Compile vertex source
    glCompileShader(vertexShader);

    // Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        ShaderUtils::printShaderLog(vertexShader);
        return 0;
    }

    // Attach vertex shader to program
    glAttachShader(programId, vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read / set fragment source
    std::string fragmentShaderSource = ShaderUtils::readShaderSource(fragShader);
    const char* fragmentShaderSource2 = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, nullptr);

    // Compile fragment source
    glCompileShader(fragmentShader);

    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile fragment shader %d!\n", fragmentShader);
        ShaderUtils::printShaderLog(fragmentShader);
        return 0;
    }

    // Attach fragment shader to program
    glAttachShader(programId, fragmentShader);

    // Link program
    glLinkProgram(programId);

    // Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
        printf("Error linking program %d!\n", programId);
        ShaderUtils::printProgramLog(programId);
        return 0;
    }

    // Individual shader objects are no longer needed
    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programId;
}

}}  // namespace Rival::Shaders
