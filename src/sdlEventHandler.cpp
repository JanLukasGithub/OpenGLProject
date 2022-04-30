#include "sdlEventHandler.h"

void SdlEventHandler::handleSdlEvent(const SDL_Event event, float32 delta) {
	switch (event.type) {
	case SDL_KEYDOWN:
		updateKeyboardInput(event.key.keysym.sym, true);
		break;
	case SDL_KEYUP:
		updateKeyboardInput(event.key.keysym.sym, false);
		break;
	case SDL_MOUSEMOTION:
		updateMouseMovement(event.motion.xrel, event.motion.yrel);
		break;
	case SDL_MOUSEBUTTONDOWN:
		updateMouseInput(event.button.button, true);
		break;
	case SDL_MOUSEBUTTONUP:
		updateMouseInput(event.button.button, false);
		break;
	default:
		break;
	}
}

void SdlEventHandler::update() {
	m_lastPressedKeys = m_pressedKeys;
	m_lastPressedMouseButtons = m_pressedMouseButtons;

	mouseXRel = 0;
	mouseYRel = 0;
}

bool SdlEventHandler::keyPressed(SDL_Keycode keycode) {
	for (int i = 0; i < m_pressedKeys.size(); i++) {
		if (m_pressedKeys.data()[i] == keycode)
			return true;
	}
	return false;
}

bool SdlEventHandler::lastKeyPressed(SDL_Keycode keycode) {
	for (int i = 0; i < m_lastPressedKeys.size(); i++) {
		if (m_lastPressedKeys.data()[i] == keycode)
			return true;
	}
	return false;
}

bool SdlEventHandler::keyJustPressed(SDL_Keycode keycode) {
	return keyPressed(keycode) && !lastKeyPressed(keycode);
}

bool SdlEventHandler::keyJustReleased(SDL_Keycode keycode) {
	return !keyPressed(keycode) && lastKeyPressed(keycode);
}

bool SdlEventHandler::mouseButtonPressed(uint8 button) {
	for (int i = 0; i < m_pressedMouseButtons.size(); i++) {
		if (m_pressedMouseButtons[i] == button)
			return true;
	}
	return false;
}

bool SdlEventHandler::lastMouseButtonPressed(uint8 button) {
	for (int i = 0; i < m_lastPressedMouseButtons.size(); i++) {
		if (m_lastPressedMouseButtons[i] == button)
			return true;
	}
	return false;
}

bool SdlEventHandler::mouseButtonJustPressed(uint8 button) {
	return mouseButtonPressed(button) && !lastMouseButtonPressed(button);
}

bool SdlEventHandler::mouseButtonJustReleased(uint8 button) {
	return !mouseButtonPressed(button) && lastMouseButtonPressed(button);
}

int32 SdlEventHandler::mouseXMovement() {
	return mouseXRel;
}

int32 SdlEventHandler::mouseYMovement() {
	return mouseYRel;
}

void SdlEventHandler::updateKeyboardInput(const SDL_Keycode key, const bool isDown) {
	auto it = std::find(m_pressedKeys.begin(), m_pressedKeys.end(), key);

	if (isDown) {
		if (it == m_pressedKeys.end())
			m_pressedKeys.push_back(key);
	} else {
		if (it == m_pressedKeys.end())
			return;

		m_pressedKeys.erase(it);
	}
}

void SdlEventHandler::updateMouseMovement(const int32 xRel, const int32 yRel) {
	mouseXRel = xRel;
	mouseYRel = yRel;
}

void SdlEventHandler::updateMouseInput(const uint8 button, const bool isDown) {
	auto it = std::find(m_pressedMouseButtons.begin(), m_pressedMouseButtons.end(), button);

	if (isDown) {
		if (it == m_pressedMouseButtons.end())
			m_pressedMouseButtons.push_back(button);
	} else {
		if (it == m_pressedMouseButtons.end())
			return;

		m_pressedMouseButtons.erase(it);
	}
}