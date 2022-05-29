#include "fpsCamera.h"

FpsCamera::FpsCamera(float fov, float width, float height) : Camera(fov, width, height) {
	onMouseMoved(0.0f, 0.0f);
}

void FpsCamera::onMouseMoved(float xRel, float yRel) {
	m_yaw += xRel * mouseSensitivity;
	m_pitch -= yRel * mouseSensitivity;
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	else if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	m_lookAt.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_lookAt.y = sin(glm::radians(m_pitch));
	m_lookAt.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_lookAt = glm::normalize(m_lookAt);
	update();
}

void FpsCamera::update() {
	Camera::m_view = glm::lookAt(Camera::m_position, Camera::m_position + m_lookAt, m_up);
	Camera::update();
}

void FpsCamera::handleInputs(SdlEventHandler* handler, float32 delta) {
	if (handler->keyPressed(SDLK_w))
		moveFront(delta);

	if (handler->keyPressed(SDLK_a))
		moveRight(-delta);

	if (handler->keyPressed(SDLK_s))
		moveFront(-delta);

	if (handler->keyPressed(SDLK_d))
		moveRight(delta);

	onMouseMoved(handler->mouseXMovement(), handler->mouseYMovement());
}

void FpsCamera::moveFront(float amount) {
	Camera::translate(
		glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * m_lookAt) * amount
		* cameraSpeed);
	update();
}

void FpsCamera::moveRight(float amount) {
	Camera::translate(
		glm::normalize(glm::cross(m_lookAt, m_up)) * amount * cameraSpeed);
	update();
}

const glm::vec3& FpsCamera::getLookAt() const noexcept {
	return m_lookAt;
}