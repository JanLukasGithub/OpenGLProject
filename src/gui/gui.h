#include <GL/glew.h>

#include "../utils.h"
#include "../buffer/vertexBuffer.h"
#include "../buffer/indexBuffer.h"
#include "../renderer/renderer.h"

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

class Gui {
protected:
	float m_zLevel;

public:
	Gui() : m_zLevel{ 0.0f } {}
	Gui(float zLevel) : m_zLevel{ zLevel } {}
	virtual ~Gui() {}

	virtual void draw() {}

	static void drawRectangle(float32 x, float32 y, float32 w, float32 h, int32 color) noexcept;
};

#endif /* GUI_GUI_H_ */
