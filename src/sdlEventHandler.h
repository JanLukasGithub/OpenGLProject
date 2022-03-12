class SdlEventHandler;

#ifndef SDLEVENTHANDLER_H_
#define SDLEVENTHANDLER_H_

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>

#include "utils.h"

class SdlEventHandler {
public:
	// Handles the SDL Event. Only call if update has been called this frame already!
	void handleSdlEvent(const SDL_Event event, float32 delta);

	// Call this once per frame before doing anything else with the class
	void update();

	// Returns if the key is pressed
	bool keyPressed(SDL_Keycode keycode);
	// Returns if the key was pressed last frame
	bool lastKeyPressed(SDL_Keycode keycode);
	// Returns if the key was just pressed this frame (=is now pressed but wasn't last frame)
	bool keyJustPressed(SDL_Keycode keycode);
	// Returns if the key was just released this frame (=isn't pressed now but was last frame)
	bool keyJustReleased(SDL_Keycode keycode);

	// Returns if the mouse button is pressed
	bool mouseButtonPressed(uint8 button);
	// Returns if the mouse button was pressed last frame
	bool lastMouseButtonPressed(uint8 button);
	// Returns if the mouse button was just pressed this frame (=is now pressed but wasn't last frame)
	bool mouseButtonJustPressed(uint8 button);
	// Returns if the mouse button was just released this frame (=isn't pressed now but was last frame)
	bool mouseButtonJustReleased(uint8 button);

	// Functions to get mouse movement
	int32 mouseXMovement();
	int32 mouseYMovement();

protected:
	// This changes the value of the inputBitfield
	void updateKeyboardInputs(const SDL_Keycode key, const bool isDown);
	// This changes the values of mouseXRel and mouseYRel
	void updateMouseMovement(const int32 xRel, const int32 yRel);
	// This changes the value of the mouseInputBitfield
	void updateMouseInputs(const Uint8 button, const bool isDown);
	// Mouse movement
	int32 mouseXRel = 0.0f, mouseYRel = 0.0f;
	// List of pressed keys
	std::vector<SDL_Keycode> m_pressedKeys = std::vector<SDL_Keycode>();
	// List of keys pressed last frame
	std::vector<SDL_Keycode> m_lastPressedKeys = std::vector<SDL_Keycode>();
	// List of mouse buttons pressed
	std::vector<uint8> m_pressedMouseButtons = std::vector<uint8>();
	// List of mouse buttons pressed last frame
	std::vector<uint8> m_lastPressedMouseButtons = std::vector<uint8>();
};

// The constants you can use for Mouse Buttons
namespace SdlMouseButtons {
	constexpr uint8 MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT;
	constexpr uint8 MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT;
	constexpr uint8 MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE;
	constexpr uint8 MOUSE_BUTTON_X1 = SDL_BUTTON_X1;
	constexpr uint8 MOUSE_BUTTON_X2 = SDL_BUTTON_X2;
}

#endif /* SDLEVENTHANDLER_H_ */
