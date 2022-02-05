/*
 * IndexBuffer.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* data, uint64 numIndices, uint8 elementSize) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &bufferId);
}

IndexBuffer* IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	return this;
}

IndexBuffer* IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return this;
}

