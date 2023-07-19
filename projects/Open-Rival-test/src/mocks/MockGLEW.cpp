#include "gfx/GlewWrapper.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

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

GLuint glCreateProgram()
{
    return 1;
}

GLuint glCreateShader(GLenum /*shaderType*/)
{
    return 1;
}

void glShaderSource(GLuint /*shader*/, GLsizei /*count*/, const GLchar** /*string*/, const GLint* /*length*/)
{
    // Do nothing!
}

void glCompileShader(GLuint /*shader*/)
{
    // Do nothing!
}

void glGetShaderiv(GLuint /*shader*/, GLenum /*pname*/, GLint* /*params*/)
{
    // Do nothing!
}

void glAttachShader(GLuint /*program*/, GLuint /*shader*/)
{
    // Do nothing!
}

void glLinkProgram(GLuint /*program*/)
{
    // Do nothing!
}

void glGetProgramiv(GLuint /*program*/, GLenum /*pname*/, GLint* /*params*/)
{
    // Do nothing!
}

GLboolean glIsProgram(GLuint /*program*/)
{
    return GL_TRUE;
}

GLboolean glIsShader(GLuint /*shader*/)
{
    return GL_TRUE;
}

void glGetProgramInfoLog(GLuint /*program*/, GLsizei /*maxLength*/, GLsizei* /*length*/, GLchar* /*infoLog*/)
{
    // Do nothing!
}

void glGetShaderInfoLog(GLuint /*shader*/, GLsizei /*maxLength*/, GLsizei* /*length*/, GLchar* /*infoLog*/)
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
