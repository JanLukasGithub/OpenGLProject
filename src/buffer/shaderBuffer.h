#ifndef SHADER_BUFFER_H_
#define SHADER_BUFFER_H_

#include <GL/glew.h>

#include <vector>

#include "../utils.h"

template<typename T>
class ShaderBuffer {
public:
    ShaderBuffer(const T* data, const uint64 numElements, const GLuint bufferBinding) : m_numElementsSize{ numElements }, m_numElementsCapacity{ numElements },
        m_bufferBinding{ bufferBinding } {
        genBuffer(data);
    }

    ShaderBuffer(const T& data, const GLuint bufferBinding) : m_numElementsSize{ 1 }, m_numElementsCapacity{ 1 }, m_bufferBinding{ bufferBinding } {
        genBuffer(&data);
    }

    ShaderBuffer(const std::vector<T>& data, const GLuint bufferBinding) : m_numElementsSize{ data.size() }, m_numElementsCapacity{ data.size() },
        m_bufferBinding{ bufferBinding } {
        genBuffer(data.data());
    }

    ShaderBuffer(const GLuint bufferBinding) : m_numElementsSize{ 0 }, m_numElementsCapacity{ 0 }, m_bufferBinding{ bufferBinding } {
        genBuffer(nullptr);
    }

    ShaderBuffer(const ShaderBuffer& buf) : m_numElementsSize{ buf.m_numElementsSize }, m_numElementsCapacity{ buf.m_numElementsCapacity },
        m_bufferBinding{ buf.m_bufferBinding } {

        genBuffer(nullptr);

        glBindBuffer(GL_COPY_READ_BUFFER, buf.m_bufferId);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, m_numElementsSize);
    }

    ShaderBuffer(ShaderBuffer<T>&& buf) : m_bufferId{ buf.m_bufferId }, m_numElementsSize{ buf.m_numElementsSize },
        m_bufferBinding{ buf.m_bufferBinding }, m_numElementsCapacity{ buf.m_numElementsCapacity } {
        buf.m_bufferId = 0;
    }

    virtual ~ShaderBuffer() {
        glDeleteBuffers(1, &m_bufferId);
    }

    ShaderBuffer& operator=(const ShaderBuffer& buf) {
        if (this == &buf)
            return *this;

        glDeleteBuffers(1, &m_bufferId);

        m_numElementsSize = buf.m_numElementsSize;
        m_numElementsCapacity = buf.m_numElementsCapacity;
        m_bufferBinding = buf.m_bufferBinding;

        genBuffer(nullptr);

        glBindBuffer(GL_COPY_READ_BUFFER, buf.m_bufferId);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, m_numElementsSize);

        return *this;
    }

    ShaderBuffer& operator=(ShaderBuffer&& buf) {
        if (this == &buf)
            return *this;

        glDeleteBuffers(1, &m_bufferId);

        m_bufferId = buf.m_bufferId;
        m_bufferBinding = buf.m_bufferBinding;
        m_numElementsCapacity = buf.m_numElementsCapacity;
        m_numElementsSize = buf.m_numElementsSize;

        buf.m_bufferId = 0;

        return *this;
    }

    void add(const T data[], const uint64 numElements) {
        GLuint oldBufferId = m_bufferId;
        uint64 oldNumElements = m_numElementsSize;

        m_numElementsSize += numElements;

        glBindBuffer(GL_COPY_READ_BUFFER, oldBufferId);

        if (m_numElementsSize <= m_numElementsCapacity) {
            glBufferSubData(GL_COPY_READ_BUFFER, oldNumElements * sizeof(T), numElements * sizeof(T), data);
            return;
        }

        m_numElementsCapacity = m_numElementsSize;

        genBuffer(nullptr);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_SHADER_STORAGE_BUFFER, 0, 0, oldNumElements * sizeof(T));
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, oldNumElements * sizeof(T), numElements * sizeof(T), data);

        glDeleteBuffers(1, &oldBufferId);
    }

    void add(const std::vector<T>& data) {
        add(data.data(), data.size());
    }

    void add(const T& data) {
        add(&data, 1);
    }

    void remove(const uint64 index, const uint64 numElements) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);

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

    void set(const T data[], const uint64 numElements, const uint64 index) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);

        glBufferSubData(GL_SHADER_STORAGE_BUFFER, index * sizeof(T), numElements * sizeof(T), data);
    }

    void set(const std::vector<T>& data, const uint64 index) {
        set(data.data(), data.size(), index);
    }

    void set(const T& data, const uint64 index) {
        set(&data, 1, index);
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

    void genBuffer(const T* data) {
        glGenBuffers(1, &m_bufferId);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bufferBinding, m_bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_numElementsCapacity * sizeof(T), data, GL_DYNAMIC_READ);
    }
};

#endif