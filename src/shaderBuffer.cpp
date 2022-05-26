#include "shaderBuffer.h"

template<typename T>
ShaderBuffer<T>::ShaderBuffer(const T* data, const uint64 numElements, GLuint bufferBinding) : m_numElementsSize{ numElements }, m_numElementsCapacity{ size },
m_bufferBinding{ bufferBinding } {
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, numElements * sizeof(T), data, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, m_bufferId);
}

template<typename T>
ShaderBuffer<T>::ShaderBuffer(ShaderBuffer<T>&& buf) : m_bufferId{ buf.m_bufferId }, m_numElementsSize{ buf.m_numElementsSize },
m_bufferBinding{ buf.m_bufferBinding }, m_numElementsCapacity{ buf.m_numElementsCapacity } {
    buf.m_bufferId = 0;
}

template<typename T>
ShaderBuffer<T>::~ShaderBuffer() {
    glDeleteBuffers(1, &m_bufferId);
}

template<typename T>
ShaderBuffer<T>& ShaderBuffer<T>::add(const T* data, const uint64 numElements) {
    GLuint oldBufferId = m_bufferId;
    uint64 oldNumElements = m_numElementsSize;

    m_numElementsSize += numElements;

    glBindBuffer(GL_COPY_READ_BUFFER, oldBufferId);

    if (m_numElementsSize <= m_numElementsCapacity) {
        glBufferSubData(GL_COPY_READ_BUFFER, oldNumElements * sizeof(T), numElements + sizeof(T), data);
        return *this;
    }

    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_numElementsSize * sizeof(T), nullptr, GL_DYNAMIC_READ);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, oldNumElements * sizeof(T));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, oldNumElements * sizeof(T), numElements * sizeof(T), data);

    glDeleteBuffers(1, &oldBufferId);

    m_numElementsCapacity = m_numElementsSize;

    return *this;
}

template<typename T>
ShaderBuffer<T>& ShaderBuffer<T>::remove(const uint64 index, const uint64 numElements) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);

    GLsizeiptr tempBufferSize = (m_numElementsSize - index - numElements) * sizeof(T);
    GLuint tempBuffer;
    glGenBuffers(1, &tempBuffer);
    glBindBuffer(GL_COPY_READ_BUFFER, tempBuffer);
    glBufferData(GL_COPY_READ_BUFFER, tempBufferSize, nullptr, GL_DYNAMIC_READ);

    glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_COPY_READ_BUFFER, (index + numElements) * sizeof(T), 0, tempBufferSize);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, index * sizeof(T), tempBufferSize);

    glDeleteBuffers(1, &tempBuffer);

    m_numElementsSize -= numElements;

    return *this;
}

template<typename T>
ShaderBuffer<T>& ShaderBuffer<T>::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);

    return *this;
}

template<typename T>
ShaderBuffer<T>& ShaderBuffer<T>::unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, 0);

    return *this;
}