#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <GL/glew.h>

#include "../utils.h"

class IndexBuffer {
private:
	GLuint m_bufferId{ 0 };
	GLsizeiptr m_size{ 0 };

public:
	IndexBuffer(const void* data, const uint64 numIndices, const uint8 elementSize) noexcept;
	IndexBuffer(const IndexBuffer& ibo) noexcept;
	IndexBuffer(IndexBuffer&& ibo) noexcept;
	virtual ~IndexBuffer() noexcept;

	IndexBuffer& operator=(const IndexBuffer& ibo);
	IndexBuffer& operator=(IndexBuffer&& ibo);

	IndexBuffer* bind() noexcept;
	IndexBuffer* unbind() noexcept;
};

#endif