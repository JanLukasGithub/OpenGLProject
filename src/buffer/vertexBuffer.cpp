#include "vertexBuffer.h"

VertexBuffer::VertexBuffer() {}

VertexBuffer::VertexBuffer(const void* data, const uint64 numVertices) noexcept : m_size{ sizeof(Vertex) * numVertices } {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const VertexBuffer& vbo) noexcept : m_size{ vbo.m_size } {
	glBindBuffer(GL_COPY_READ_BUFFER, vbo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);
}

VertexBuffer::VertexBuffer(VertexBuffer&& vbo) noexcept : m_size{vbo.m_size}, m_bufferId{ vbo.m_bufferId } {
	vbo.m_bufferId = 0;
}

VertexBuffer::~VertexBuffer() noexcept {
	glDeleteBuffers(1, &m_bufferId);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& vbo) {
	if (&vbo == this)
		return *this;

	m_size = vbo.m_size;

	glDeleteBuffers(1, &m_bufferId);

	glBindBuffer(GL_COPY_READ_BUFFER, vbo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

	return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vbo) {
	if (&vbo == this)
		return *this;

	glDeleteBuffers(1, &m_bufferId);

	m_size = vbo.m_size;
	m_bufferId = vbo.m_bufferId;

	vbo.m_bufferId = 0;

	return *this;
}

VertexBuffer& VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	return *this;
}
