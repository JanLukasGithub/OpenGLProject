/*
 * Camera.h
 *
 *  Created on: Jul 21, 2021
 *      Author: jan
 */

class Camera;

#ifndef CAMERAS_CAMERA_H_
#define CAMERAS_CAMERA_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"
#pragma GCC diagnostic pop

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
