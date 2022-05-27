#ifndef LIGHTS_LIGHTS_H_
#define LIGHTS_LIGHTS_H_

#include "../lib/glm/glm.hpp"

#include "shader.h"

namespace LIGHTS {
	inline int directionalLightDirectionUniformLocation;
	inline int pointLightPositionUniformLocation;
	inline int spotLightPositionUniformLocation;
	inline int spotLightDirectionUniformLocation;

	inline void initUniforms(const Shader* shader) {
		directionalLightDirectionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_directionalLight.direction");
		pointLightPositionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_pointLight.position");
		spotLightPositionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_spotLight.position");
		spotLightDirectionUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_spotLight.direction");
	}
}

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec3 ambientColor;
};

struct PointLight {
	glm::vec4 position;
	glm::vec3 color;
	glm::vec3 ambientColor;
	float linear, quadratic;
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec3 ambientColor;
	float linear, quadratic;
	float innerCone, outerCone;
};

#endif