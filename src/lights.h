#ifndef LIGHTS_LIGHTS_H_
#define LIGHTS_LIGHTS_H_

#include "../lib/glm/glm.hpp"

struct DirectionalLight {
	int directionUniformLocation;
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec3 ambientColor;
};

struct PointLight {
	int positionUniformLocation;
	glm::vec4 position;
	glm::vec3 color;
	glm::vec3 ambientColor;
	float linear, quadratic;
};

struct SpotLight {
	int positionUniformLocation;
	int directionUniformLocation;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec3 ambientColor;
	float linear, quadratic;
	float innerCone, outerCone;
};

#endif