#ifndef SHADER_BUFFER_H_
#define SHADER_BUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class ShaderBuffer {
private:
    GLuint m_bufferId{ 0 };
    GLsizeiptr m_size;
    GLsizeiptr m_capacity;
    GLuint m_bufferBinding;

public:
    ShaderBuffer(const void* data, const uint64 size, const GLuint bufferBinding);
    ShaderBuffer(ShaderBuffer&& buf);
    virtual ~ShaderBuffer();

    ShaderBuffer& add(const void* newData, const uint64 size);
    ShaderBuffer& remove(const uint64 index, const uint64 size);

    ShaderBuffer& bind();
    ShaderBuffer& unbind();
};

#endif