/*
 * Gui.cpp
 *
 *  Created on: Aug 13, 2021
 *      Author: jan
 */

#include "Gui.h"

//void Gui::drawRectangle(int x, int y, int w, int h) {
//	Vertex vertices[] = {
//		Vertex{(float)x, (float)y, 1.0f},
//		Vertex{(float)x, (float)y + h, 1.0f},
//		Vertex{(float)x + w, (float)y + h, 1.0f},
//		Vertex{(float)x + w, (float)y, 1.0f}
//	};
//	uint64 indices[] = {
//			0, 1, 3,
//			1, 2, 3
//	};
//	VertexBuffer* vBuffer = new VertexBuffer(vertices, 4);
//	IndexBuffer* iBuffer = new IndexBuffer(indices, 6, sizeof(uint64));
//	vBuffer->bind();
//	iBuffer->bind();
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	iBuffer->unbind();
//	vBuffer->unbind();
//	delete iBuffer, vBuffer;
//}
