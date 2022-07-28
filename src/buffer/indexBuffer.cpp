#include "indexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, const uint64 numIndices, const uint8 elementSize) noexcept : m_size{ numIndices * elementSize } {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const IndexBuffer& ibo) noexcept : m_size{ ibo.m_size } {
	glBindBuffer(GL_COPY_READ_BUFFER, ibo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_size);
}

IndexBuffer::IndexBuffer(IndexBuffer&& ibo) noexcept : m_size{ ibo.m_size }, m_bufferId{ ibo.m_bufferId } {
	ibo.m_bufferId = 0;
}

IndexBuffer::~IndexBuffer() noexcept {
	glDeleteBuffers(1, &m_bufferId);
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& ibo) {
	if (&ibo == this)
		return *this;

	glDeleteBuffers(1, &m_bufferId);

	glBindBuffer(GL_COPY_READ_BUFFER, ibo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_size);

	return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ibo) {
	if (&ibo == this)
		return *this;

	glDeleteBuffers(1, &m_bufferId);

	m_size = ibo.m_size;
	m_bufferId = ibo.m_bufferId;

	ibo.m_bufferId = 0;

	return *this;
}

IndexBuffer* IndexBuffer::bind() noexcept {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	return this;
}

IndexBuffer* IndexBuffer::unbind() noexcept {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return this;
}