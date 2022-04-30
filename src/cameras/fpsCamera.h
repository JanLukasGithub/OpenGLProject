#ifndef CAMERAS_FPSCAMERA_H_
#define CAMERAS_FPSCAMERA_H_

#include "camera.h"

class FpsCamera : public Camera {
protected:
	float yaw{ -90.0f };
	float pitch{ 0.0f };
	glm::vec3 lookAt{};
	const float mouseSensitivity = 0.3f;
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };

public:
	static constexpr float cameraSpeed = 5.0f;

	FpsCamera(float fov, float width, float height);

	void onMouseMoved(float xRel, float yRel);
	virtual void update() override;
	virtual void handleInputs(SdlEventHandler* handler, float32 delta) override;

	void moveFront(float amount);
	void moveRight(float amount);
};

#endif