#include "indexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, const uint64 numIndices, const uint8 elementSize) noexcept : m_size{ numIndices * elementSize } {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const IndexBuffer& ibo) noexcept : m_size{ ibo.m_size } {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glCopyBufferSubData(ibo.m_bufferId, m_bufferId, 0, 0, m_size);
}

IndexBuffer::~IndexBuffer() noexcept {
	glDeleteBuffers(1, &m_bufferId);
}

IndexBuffer* IndexBuffer::bind() noexcept {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	return this;
}

IndexBuffer* IndexBuffer::unbind() noexcept {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return this;
}

