/*
 * GuiScreen.h
 *
 *  Created on: Aug 13, 2021
 *      Author: jan
 */

#ifndef GUI_GUISCREEN_H_
#define GUI_GUISCREEN_H_

#include "Gui.h"

#include <SDL2/SDL.h>

class GuiScreen: public Gui {
public:
	GuiScreen(SDL_Window* window);
	GuiScreen(SDL_Window* window, float zLevel);
	GuiScreen(int height, int width);
	GuiScreen(float zLevel, int height, int width);
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

protected:
	// Size in pixels
	int m_width, m_height;
};

#endif /* GUI_GUISCREEN_H_ */
