#include "vertexBuffer.h"

void initVertexAttribArray() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
}

VertexBuffer::VertexBuffer() {}

VertexBuffer::VertexBuffer(const void* data, const uint64 numVertices) noexcept : m_size{ sizeof(Vertex) * numVertices } {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);

	initVertexAttribArray();
}

VertexBuffer::VertexBuffer(const VertexBuffer& vbo) noexcept : m_size{ vbo.m_size } {
	glBindBuffer(GL_COPY_READ_BUFFER, vbo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

	initVertexAttribArray();
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

	glDeleteBuffers(1, &m_bufferId);

	glBindBuffer(GL_COPY_READ_BUFFER, vbo.m_bufferId);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

	initVertexAttribArray();

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