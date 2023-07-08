#include "gfx/ShaderUtils.h"

#include "utils/SDLWrapper.h"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>

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
        printf("Name %d is not a program\n", program);
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
        printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

void printShaderLog(const GLuint shader)
{

    if (!glIsShader(shader))
    {
        printf("Name %d is not a shader\n", shader);
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
        printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

}}  // namespace Rival::ShaderUtils
