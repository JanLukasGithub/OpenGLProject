#ifndef SHADER_BUFFER_H_
#define SHADER_BUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class ShaderBuffer {
private:
    GLuint m_bufferId{ 0 };
    const GLsizeiptr m_size;
    GLuint m_bufferBinding;

public:
    ShaderBuffer(const void* data, const uint64 size, GLuint bufferBinding);
    ShaderBuffer(ShaderBuffer&& buf);
    virtual ~ShaderBuffer();

    ShaderBuffer& bind();
    ShaderBuffer& unbind();
};

#endif