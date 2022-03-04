#include "GuiScreen.h"

 // Values can't be initialized in initializer list, prevent the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
// Constructors
GuiScreen::GuiScreen(SDL_Window* window) : Gui() {
	SDL_GetWindowSize(window, &m_width, &m_height);
}

GuiScreen::GuiScreen(SDL_Window* window, float zLevel) : Gui(zLevel) {
	SDL_GetWindowSize(window, &m_width, &m_height);
}
#pragma GCC diagnostic pop

GuiScreen::GuiScreen(int height, int width) : Gui(), m_width{ width }, m_height{ height } {}

GuiScreen::GuiScreen(float zLevel, int height, int width) : Gui(zLevel), m_width{ width }, m_height{ height } {}

