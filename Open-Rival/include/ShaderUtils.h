#pragma once

#include <gl/glew.h>

#include <string>

namespace Rival { namespace ShaderUtils {

std::string readShaderSource(const char* filename);

void printProgramLog(const GLuint program);

void printShaderLog(const GLuint shader);

}}  // namespace Rival::ShaderUtils
