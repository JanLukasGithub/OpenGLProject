/*
 * FpsCamera.h
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

class FpsCamera;

#ifndef CAMERAS_FPSCAMERA_H_
#define CAMERAS_FPSCAMERA_H_

#include "Camera.h"

class FpsCamera : public Camera {
public:
	// Setting this to a negative value reverses horizontal camera movement
	static constexpr float cameraSpeed = 5.0f;

	FpsCamera(float fov, float width, float height);

	void onMouseMoved(float xRel, float yRel);

	virtual void update() override;

	virtual void handleInputs(SdlEventHandler* handler, float32 delta) override;

	// Negative amount means moving backwards, multiplies by cameraSpeed on it's own
	void moveFront(float amount);

	// Negative amount means moving to the left, multiplies by cameraSpeed on it's own
	void moveRight(float amount);

protected:
	float yaw{ -90.0f };
	float pitch{ 0.0f };
	glm::vec3 lookAt{};
	const float mouseSensitivity = 0.3f;
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
};

#endif /* CAMERAS_FPSCAMERA_H_ */
