#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class VertexBuffer {
private:
	GLuint m_bufferId{ 0 };
	GLuint m_vao{ 0 };

public:
	VertexBuffer(void* data, uint64 numVertices, bool* hasNormalMap);
	virtual ~VertexBuffer();

	VertexBuffer* bind();
	VertexBuffer* unbind();
};

#endif /* VERTEXBUFFER_H_ */
