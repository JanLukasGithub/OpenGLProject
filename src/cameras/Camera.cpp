/*
 * Camera.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

#include "Camera.h"

Camera::Camera(float fov, float width, float height) : projection{ glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f) }, viewProj{ projection * view } {}

Camera::Camera(float fov, float width, float height, float near, float far) : projection{ glm::perspective(fov / 2.0f, width / height, near, far) },
viewProj{ projection * view } {}

Camera::~Camera() {
}

glm::mat4 Camera::getViewProjection() {
	return viewProj;
}

glm::mat4 Camera::getView() {
	return view;
}

void Camera::update() {
	viewProj = projection * view;
}

void Camera::translate(glm::vec3 vec3) {
	position += vec3;
	view = glm::translate(view, vec3 * -1.0f);
}

// Ignore parameters not being used as this function does nothing
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Camera::handleInputs(SdlEventHandler* handler, float32 delta) {}
#pragma GCC diagnostic pop
