#ifndef LIGHTS_LIGHTS_H_
#define LIGHTS_LIGHTS_H_

#include <vector>

#include "../lib/glm/glm.hpp"

#include "shader.h"
#include "utils.h"
#include "buffer/shaderBuffer.h"

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

// Wrapper around 3 ShaderBuffers. To add or remove lights, access the buffers and use their interface
class Lights {
public:
	Lights();
	Lights(const DirectionalLight& dirLight, const PointLight& pointLight, const SpotLight& spotLight);
	Lights(const std::vector<DirectionalLight>& dirLights, const std::vector<PointLight>& pointLights, const std::vector<SpotLight>& spotLights);
	Lights(const Lights& lights) = default;
	Lights& operator=(const Lights& lights);
	Lights(Lights&& lights) = default;
	virtual ~Lights() = default;

	void bind() const noexcept;
	void unbind() const noexcept;

	ShaderBuffer<DirectionalLight>& getDirectionalLightBuffer();
	ShaderBuffer<PointLight>& getPointLightBuffer();
	ShaderBuffer<SpotLight>& getSpotLightBuffer();
private:
	ShaderBuffer<DirectionalLight> m_directionalLightBuffer;
	ShaderBuffer<PointLight> m_pointLightBuffer;
	ShaderBuffer<SpotLight> m_spotLightBuffer;
};

#endif