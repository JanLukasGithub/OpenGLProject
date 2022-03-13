#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class IndexBuffer {
private:
	GLuint m_bufferId{ 0 };

public:
	IndexBuffer(void* data, uint64 numIndices, uint8 elementSize);
	virtual ~IndexBuffer();

	IndexBuffer* bind();
	IndexBuffer* unbind();
};

#endif /* INDEXBUFFER_H_ */
