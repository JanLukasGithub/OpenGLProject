#ifndef GUI_GUISCREEN_H_
#define GUI_GUISCREEN_H_

#include <SDL2/SDL.h>

#include "gui.h"

class GuiScreen : public Gui {
protected:
	// Size in pixels
	int m_width, m_height;

public:
	GuiScreen(SDL_Window* window);
	GuiScreen(SDL_Window* window, float zLevel);
	GuiScreen(int height, int width);
	GuiScreen(int height, int width, float zLevel);
	virtual ~GuiScreen() {}

	int getHeight() {
		return m_height;
	}

	void setHeight(int height) {
		m_height = height;
	}

	int getWidth() {
		return m_width;
	}

	void setWidth(int width) {
		m_width = width;
	}

	virtual void drawScreen() {}

	void draw() override {
		drawScreen();
	}
};

#endif /* GUI_GUISCREEN_H_ */
