#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <GL/glew.h>

#include "../utils.h"

class VertexBuffer {
private:
	GLuint m_bufferId{ 0 };
	GLuint m_vao{ 0 };
	GLsizeiptr m_size{ 0 };

public:
	VertexBuffer(const void* data, const uint64 numVertices) noexcept;
	VertexBuffer(const VertexBuffer& vbo) noexcept;
	VertexBuffer(VertexBuffer&& vbo) noexcept;
	virtual ~VertexBuffer() noexcept;

	VertexBuffer& operator=(const VertexBuffer& vbo);
	VertexBuffer& operator=(VertexBuffer&& vbo);

	VertexBuffer* bind() noexcept;
	VertexBuffer* unbind() noexcept;
};

#endif
