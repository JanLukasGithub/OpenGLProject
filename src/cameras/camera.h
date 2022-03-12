class Camera;

#ifndef CAMERAS_CAMERA_H_
#define CAMERAS_CAMERA_H_

#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../utils.h"

// Forward declaration
class SdlEventHandler;

class Camera {
public:
	Camera(float fov, float width, float height);

	Camera(float fov, float width, float height, float near, float far);

	virtual ~Camera();

	glm::mat4 getViewProjection();

	glm::mat4 getView();

	virtual void update();

	virtual void translate(glm::vec3 vec3);

	virtual void handleInputs(SdlEventHandler* handler, float32 delta);

protected:
	glm::vec3 position{ 0.0f };
	glm::mat4 projection;
	glm::mat4 view{ 1.0f };
	glm::mat4 viewProj;
};

#endif /* CAMERAS_CAMERA_H_ */
