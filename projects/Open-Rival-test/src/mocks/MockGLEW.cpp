#include "gfx/GlewWrapper.h"

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

void glBlendFunc(GLenum /*sfactor*/, GLenum /*dfactor*/)
{
    // Do nothing!
}

void glFrontFace(GLenum /*mode*/)
{
    // Do nothing!
}

void glEnable(GLenum /*cap*/)
{
    // Do nothing!
}

GLint glGetUniformLocation(GLuint /*program*/, const GLchar* /*name*/)
{
    return 1;
}

GLint glGetAttribLocation(GLuint /*program*/, const GLchar* /*name*/)
{
    return 1;
}

void glGenVertexArrays(int /*n*/, GLuint* /*arrays*/)
{
    // Do nothing!
}

void glBindVertexArray(GLuint /*array*/)
{
    // Do nothing!
}

void glGenBuffers(GLsizei /*n*/, GLuint* /*buffers*/)
{
    // Do nothing!
}

void glBindBuffer(GLenum /*target*/, GLuint /*buffer*/)
{
    // Do nothing!
}

void glBufferData(GLenum /*target*/, GLsizeiptr /*size*/, const void* /*data*/, GLenum /*usage*/)
{
    // Do nothing!
}

void glVertexAttribPointer(
        GLuint /*index*/,
        GLint /*size*/,
        GLenum /*type*/,
        GLboolean /*normalized*/,
        GLsizei /*stride*/,
        const void* /*pointer*/)
{
    // Do nothing!
}

void glEnableVertexAttribArray(GLuint /*index*/)
{
    // Do nothing!
}

void glPixelStorei(GLenum /*pname*/, GLint /*param*/)
{
    // Do nothing!
}

void glClearColor(GLclampf /*red*/, GLclampf /*green*/, GLclampf /*blue*/, GLclampf /*alpha*/)
{
    // Do nothing!
}

const GLubyte* glewGetErrorString(GLenum /*error*/)
{
    return nullptr;
}

const GLubyte* glewGetString(GLenum /*name*/)
{
    return nullptr;
}
