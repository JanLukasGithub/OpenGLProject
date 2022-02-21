/*
 * Gui.h
 *
 *  Created on: Aug 13, 2021
 *      Author: jan
 */

class Gui;

#include <GL/glew.h>

#include "../utils.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../renderer/OpenGL/OpenGLRenderer.h"

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

class Gui {
public:
	Gui() : m_zLevel{ 0.0f } {}
	Gui(float zLevel) : m_zLevel{ zLevel } {}
	virtual ~Gui() {}

	virtual void draw() {}

	static void drawRectangle(int32 x, int32 y, int32 w, int32 h, int32 color) noexcept;

protected:
	float m_zLevel;
};

#endif /* GUI_GUI_H_ */
