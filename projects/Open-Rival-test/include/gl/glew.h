/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub definitions for unit testing  !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// Including the real `glew.h` is problematic due to its arcane use of macros.
// So, we supply our own header file instead with all required definitions.

// If you find yourself needing to add additional definitions here then it is worth asking the question: why are these
// needed? In many cases these definitions are needed by renderers, which can often be omitted from test code entirely
// by replacing the dependant headers with stripped-down versions (see the test version of GameRenderer.h), and
// excluding the renderer's source file from the CMake build.

#pragma once

#include <cstddef>

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_CULL_FACE 0x0B44
#define GL_BLEND 0x0BE2
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_TEXTURE_2D 0x0DE1
#define GL_FLOAT 0x1406
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
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
#define GL_CW 0x0900

GLenum glewInit(void);
void glBlendFunc(GLenum sfactor, GLenum dfactor);
void glFrontFace(GLenum mode);
void glEnable(GLenum cap);

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
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

/* error codes */
#define GLEW_OK 0

extern GLboolean glewExperimental;
const GLubyte* glewGetErrorString(GLenum error);
const GLubyte* glewGetString(GLenum name);
