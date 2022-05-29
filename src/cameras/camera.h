#ifndef CAMERAS_CAMERA_H_
#define CAMERAS_CAMERA_H_

#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../utils.h"
#include "../sdlEventHandler.h"

class Camera {
protected:
	glm::vec3 m_position{ 0.0f };
	glm::mat4 m_projection;
	glm::mat4 m_view{ 1.0f };
	glm::mat4 m_viewProj;

public:
	Camera(float fov, float width, float height);
	Camera(float fov, float width, float height, float near, float far);
	virtual ~Camera();

	virtual void update();
	virtual void translate(glm::vec3 vec3);
	virtual void handleInputs(SdlEventHandler* handler, float32 delta);

	const glm::mat4& getProjection() const noexcept;
	const glm::mat4& getViewProjection() const noexcept;
	const glm::mat4& getView() const noexcept;
	const glm::vec3& getPosition() const noexcept;
};

#endif