#ifndef CAMERAS_FPSCAMERA_H_
#define CAMERAS_FPSCAMERA_H_

#include "camera.h"

class FpsCamera : public Camera {
protected:
	static constexpr float mouseSensitivity = 0.3f;

	float m_yaw{ -90.0f };
	float m_pitch{ 0.0f };
	glm::vec3 m_lookAt{};
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };

	float m_currentSpeed{ cameraSpeed };

public:
	static constexpr float cameraSpeed = 150.0f;
	static constexpr float sprintingCameraSpeed = 300.0f;

	FpsCamera(float fov, float width, float height);

	void onMouseMoved(float xRel, float yRel);
	virtual void update() override;
	virtual void handleInputs(SdlEventHandler* handler, float32 delta) override;

	void moveFront(float amount);
	void moveRight(float amount);

	const glm::vec3& getLookAt() const noexcept;
};

#endif