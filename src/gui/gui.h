class Gui;

#include <GL/glew.h>

#include "../utils.h"
#include "../vertexBuffer.h"
#include "../indexBuffer.h"
#include "../renderer/renderer.h"

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

class Gui {
public:
	Gui() : m_zLevel{ 0.0f } {}
	Gui(float zLevel) : m_zLevel{ zLevel } {}
	virtual ~Gui() {}

	virtual void draw() {}

	static void drawRectangle(float32 x, float32 y, float32 w, float32 h, int32 color) noexcept;

protected:
	float m_zLevel;
};

#endif /* GUI_GUI_H_ */
