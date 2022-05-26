#include "shaderBuffer.h"

ShaderBuffer::ShaderBuffer(const void* data, const uint64 size, GLuint bufferBinding) : m_size{ size }, m_capacity{ size }, m_bufferBinding{ bufferBinding } {
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, m_bufferId);
}

ShaderBuffer::ShaderBuffer(ShaderBuffer&& buf) : m_bufferId{ buf.m_bufferId }, m_size{ buf.m_size }, m_bufferBinding{ buf.m_bufferBinding }, m_capacity{ buf.m_capacity } {
    buf.m_bufferId = 0;
}

ShaderBuffer::~ShaderBuffer() {
    glDeleteBuffers(1, &m_bufferId);
}

ShaderBuffer& ShaderBuffer::add(const void* newData, const uint64 addedSize) {
    GLuint oldBufferId = m_bufferId;
    GLsizeiptr oldBufferSize = m_size;

    m_size = oldBufferSize + addedSize;

    glBindBuffer(GL_COPY_READ_BUFFER, oldBufferId);

    if (m_size <= m_capacity) {
        glBufferSubData(GL_COPY_READ_BUFFER, oldBufferSize, addedSize, newData);
        return *this;
    }

    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, oldBufferSize + addedSize, nullptr, GL_DYNAMIC_READ);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, oldBufferSize);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, oldBufferSize, addedSize, newData);

    glDeleteBuffers(1, &oldBufferId);

    m_capacity = m_size;

    return *this;
}

ShaderBuffer& ShaderBuffer::remove(const uint64 offset, const uint64 size) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);

    GLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);
    glBindBuffer(GL_COPY_READ_BUFFER, tempBuffer);
    glBufferData(GL_COPY_READ_BUFFER, m_size - offset - size, nullptr, GL_DYNAMIC_READ);

    glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_COPY_READ_BUFFER, offset + size, 0, m_size - offset - size);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, offset, m_size - offset - size);

    glDeleteBuffers(1, &tempBuffer);

    m_size -= size;

    return *this;
}

ShaderBuffer& ShaderBuffer::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);

    return *this;
}

ShaderBuffer& ShaderBuffer::unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, 0);

    return *this;
}