#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class VertexBuffer {
private:
	GLuint m_bufferId{ 0 };
	GLuint m_vao{ 0 };

public:
	VertexBuffer(const void* data, const uint64 numVertices) noexcept;
	virtual ~VertexBuffer() noexcept;

	VertexBuffer* bind() noexcept;
	VertexBuffer* unbind() noexcept;
};

#endif /* VERTEXBUFFER_H_ */
