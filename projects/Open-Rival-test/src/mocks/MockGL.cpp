#include "gfx/GLWrapper.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

GLboolean glewExperimental = false;

GLenum glewInit(void)
{
    return GLEW_OK;
}

void glBlendFunc(GLenum /*sfactor*/, GLenum /*dfactor*/) {}

void glFrontFace(GLenum /*mode*/) {}

void glEnable(GLenum /*cap*/) {}

GLint glGetUniformLocation(GLuint /*program*/, const GLchar* /*name*/)
{
    return 1;
}

GLint glGetAttribLocation(GLuint /*program*/, const GLchar* /*name*/)
{
    return 1;
}

void glGenVertexArrays(int /*n*/, GLuint* /*arrays*/) {}

void glBindVertexArray(GLuint /*array*/) {}

void glGenBuffers(GLsizei /*n*/, GLuint* /*buffers*/) {}

void glBindBuffer(GLenum /*target*/, GLuint /*buffer*/) {}

void glBufferData(GLenum /*target*/, GLsizeiptr /*size*/, const void* /*data*/, GLenum /*usage*/) {}

void glVertexAttribPointer(
        GLuint /*index*/,
        GLint /*size*/,
        GLenum /*type*/,
        GLboolean /*normalized*/,
        GLsizei /*stride*/,
        const void* /*pointer*/)
{
}

void glEnableVertexAttribArray(GLuint /*index*/) {}

void glPixelStorei(GLenum /*pname*/, GLint /*param*/) {}

void glClearColor(GLclampf /*red*/, GLclampf /*green*/, GLclampf /*blue*/, GLclampf /*alpha*/) {}

const GLubyte* glewGetErrorString(GLenum /*error*/)
{
    return nullptr;
}

const GLubyte* glewGetString(GLenum /*name*/)
{
    return nullptr;
}
