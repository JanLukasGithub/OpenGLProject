#include "shaderBuffer.h"

ShaderBuffer::ShaderBuffer(const void* data, const uint64 size, GLuint bufferBinding) : m_size{ size }, m_bufferBinding{ bufferBinding } {
    glGenBuffers(1, &m_bufferId);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, m_bufferId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_READ);
}

ShaderBuffer::ShaderBuffer(ShaderBuffer&& buf) : m_bufferId{ buf.m_bufferId }, m_size{ buf.m_size }, m_bufferBinding{ buf.m_bufferBinding } {
    buf.m_bufferId = 0;
}

ShaderBuffer::~ShaderBuffer() {
    glDeleteBuffers(1, &m_bufferId);
}

ShaderBuffer& ShaderBuffer::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);

    return *this;
}

ShaderBuffer& ShaderBuffer::unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, 0);

    return *this;
}