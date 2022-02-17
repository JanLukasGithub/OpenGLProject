/*
 * IndexBuffer.h
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <GL/glew.h>

#include "utils.h"

class IndexBuffer {
public:
	IndexBuffer(void* data, uint64 numIndices, uint8 elementSize);
	virtual ~IndexBuffer();

	IndexBuffer* bind();
	IndexBuffer* unbind();
private:
	GLuint bufferId{ 0 };
};

#endif /* INDEXBUFFER_H_ */
