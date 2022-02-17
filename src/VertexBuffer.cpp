/*
 * VertexBuffer.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* data, uint64 numVertices, bool* hasNormalMap) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

	glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_bufferId);
}

VertexBuffer* VertexBuffer::bind() {
	glBindVertexArray(m_vao);
	return this;
}

VertexBuffer* VertexBuffer::unbind() {
	glBindVertexArray(0);
	return this;
}

