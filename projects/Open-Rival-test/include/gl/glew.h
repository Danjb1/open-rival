#pragma once

#include <cstddef>

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub definitions for unit testing  !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef float GLfloat;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_FLOAT 0x1406
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

GLuint glCreateProgram();

GLuint glCreateShader(GLenum shaderType);

void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);

void glCompileShader(GLuint shader);

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);

void glAttachShader(GLuint program, GLuint shader);

void glLinkProgram(GLuint program);

void glGetProgramiv(GLuint program, GLenum pname, GLint* params);

GLboolean glIsProgram(GLuint program);

GLboolean glIsShader(GLuint shader);

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);

GLint glGetUniformLocation(GLuint program, const GLchar* name);

GLint glGetAttribLocation(GLuint program, const GLchar* name);

void glGenVertexArrays(int n, GLuint* arrays);

void glBindVertexArray(GLuint array);

void glGenBuffers(GLsizei n, GLuint* buffers);

void glBindBuffer(GLenum target, GLuint buffer);

void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

void glVertexAttribPointer(
        GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

void glEnableVertexAttribArray(GLuint index);

void glPixelStorei(GLenum pname, GLint param);
