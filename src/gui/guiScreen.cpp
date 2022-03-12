#include "guiScreen.h"

// Constructors
GuiScreen::GuiScreen(SDL_Window* window) : Gui() {
	SDL_GetWindowSize(window, &m_width, &m_height);
}

GuiScreen::GuiScreen(SDL_Window* window, float zLevel) : Gui(zLevel) {
	SDL_GetWindowSize(window, &m_width, &m_height);
}

GuiScreen::GuiScreen(int height, int width) : Gui(), m_width{ width }, m_height{ height } {}

GuiScreen::GuiScreen(float zLevel, int height, int width) : Gui(zLevel), m_width{ width }, m_height{ height } {}

