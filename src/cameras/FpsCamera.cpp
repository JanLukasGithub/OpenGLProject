/*
 * FpsCamera.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#include "FpsCamera.h"

#include "../SdlEventHandler.h"

FpsCamera::FpsCamera(float fov, float width, float height) : Camera(fov, width, height) {
	onMouseMoved(0.0f, 0.0f);
}

void FpsCamera::onMouseMoved(float xRel, float yRel) {
	yaw += xRel * mouseSensitivity;
	pitch -= yRel * mouseSensitivity;
	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	lookAt = glm::normalize(front);
	update();
}

void FpsCamera::update() {
	Camera::view = glm::lookAt(Camera::position, Camera::position + lookAt, up);
	Camera::update();
}

void FpsCamera::moveFront(float amount) {
	Camera::translate(
		glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount
		* cameraSpeed);
	update();
}

void FpsCamera::moveRight(float amount) {
	Camera::translate(
		glm::normalize(glm::cross(lookAt, up)) * amount * cameraSpeed);
	update();
}

void FpsCamera::handleInputs(SdlEventHandler* handler, float32 delta) {
	// Move the camera
	if (handler->keyPressed(SDLK_w))
		moveFront(delta);

	if (handler->keyPressed(SDLK_a))
		moveRight(-delta);

	if (handler->keyPressed(SDLK_s))
		moveFront(-delta);

	if (handler->keyPressed(SDLK_d))
		moveRight(delta);

	onMouseMoved(handler->mouseXMovement(), handler->mouseYMovement());

	Camera::handleInputs(handler, delta);
}
