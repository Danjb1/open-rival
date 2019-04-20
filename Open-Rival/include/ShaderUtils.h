#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>

#include <gl\glew.h>

namespace Rival {

    std::string readShaderSource(const char* filename);

    void printProgramLog(GLuint program);

    void printShaderLog(GLuint shader);

}

#endif // SHADER_UTILS_H
