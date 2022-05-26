#ifndef SHADER_BUFFER_H_
#define SHADER_BUFFER_H_

#include <GL/glew.h>

#include <vector>

#include "utils.h"

template<typename T>
class ShaderBuffer {
public:
    ShaderBuffer(const T* data, const uint64 numElements, const GLuint bufferBinding) : m_numElementsSize{ numElements }, m_numElementsCapacity{ numElements },
        m_bufferBinding{ bufferBinding } {
        glGenBuffers(1, &m_bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, numElements * sizeof(T), data, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, m_bufferId);
    }

    ShaderBuffer(ShaderBuffer<T>&& buf) : m_bufferId{ buf.m_bufferId }, m_numElementsSize{ buf.m_numElementsSize },
        m_bufferBinding{ buf.m_bufferBinding }, m_numElementsCapacity{ buf.m_numElementsCapacity } {
        buf.m_bufferId = 0;
    }

    virtual ~ShaderBuffer() {
        glDeleteBuffers(1, &m_bufferId);
    }

    void add(const T* data, const uint64 numElements) {
        GLuint oldBufferId = m_bufferId;
        uint64 oldNumElements = m_numElementsSize;

        m_numElementsSize += numElements;

        glBindBuffer(GL_COPY_READ_BUFFER, oldBufferId);

        if (m_numElementsSize <= m_numElementsCapacity) {
            glBufferSubData(GL_COPY_READ_BUFFER, oldNumElements * sizeof(T), numElements + sizeof(T), data);
            return;
        }

        glGenBuffers(1, &m_bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_numElementsSize * sizeof(T), nullptr, GL_DYNAMIC_READ);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, oldNumElements * sizeof(T));
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, oldNumElements * sizeof(T), numElements * sizeof(T), data);

        glDeleteBuffers(1, &oldBufferId);

        m_numElementsCapacity = m_numElementsSize;
    }

    void add(const std::vector<T>& data) {
        add(data.data(), data.size());
    }

    void remove(const uint64 index, const uint64 numElements) {
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
    }

    void bind() const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);
    }
    
    void unbind() const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, 0);
    }

    uint64 getSize() const {
        return m_numElementsSize;
    }
    uint64 getCapacity() const {
        return m_numElementsCapacity;
    }

private:
    GLuint m_bufferId{ 0 };
    uint64 m_numElementsSize;
    uint64 m_numElementsCapacity;
    GLuint m_bufferBinding;
};

#endif