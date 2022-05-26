#ifndef SHADER_BUFFER_H_
#define SHADER_BUFFER_H_

#include <GL/glew.h>

#include <vector>

#include "utils.h"

template<typename T>
class ShaderBuffer {
public:
    ShaderBuffer(const T* data, const uint64 numElements, const GLuint bufferBinding);
    ShaderBuffer(ShaderBuffer<T>&& buf);
    virtual ~ShaderBuffer();

    ShaderBuffer& add(const T* data, const uint64 numElements);
    ShaderBuffer& add(const std::vector<T>& data);
    ShaderBuffer& remove(const uint64 index, const uint64 numElements);

    ShaderBuffer& bind();
    ShaderBuffer& unbind();

private:
    GLuint m_bufferId{ 0 };
    uint64 m_numElementsSize;
    uint64 m_numElementsCapacity;
    GLuint m_bufferBinding;
};

#endif