#include "gfx/ShaderUtils.h"

#include "utils/SDLWrapper.h"

#include <stdio.h>

#include <fstream>
#include <sstream>

#include "utils/LogUtils.h"

namespace Rival { namespace ShaderUtils {

std::string readShaderSource(const char* filename)
{
    std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void printProgramLog(const GLuint program)
{

    if (!glIsProgram(program))
    {
        LOG_WARN("Name {} is not a program", program);
        return;
    }

    // Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
        // Print Log
        LOG_WARN("{}", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

void printShaderLog(const GLuint shader)
{

    if (!glIsShader(shader))
    {
        LOG_WARN("Name {} is not a shader", shader);
    }

    // Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
        // Print Log
        LOG_WARN("{}", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

}}  // namespace Rival::ShaderUtils
