#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>

#include <gl\glew.h>

namespace rival {

std::string readShaderSource(const char* filename);

void printProgramLog(GLuint program);

void printShaderLog(GLuint shader);

} // namespace rival

#endif  // SHADER_UTILS_H
