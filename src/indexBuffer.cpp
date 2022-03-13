#include "indexBuffer.h"

IndexBuffer::IndexBuffer(void* data, uint64 numIndices, uint8 elementSize) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_bufferId);
}

IndexBuffer* IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	return this;
}

IndexBuffer* IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return this;
}

