#ifndef LIGHTS_LIGHTS_H_
#define LIGHTS_LIGHTS_H_

#include <vector>

#include "../lib/glm/glm.hpp"

#include "shader.h"
#include "utils.h"
#include "buffer/shaderBuffer.h"

struct DirectionalLight {
	glm::vec4 direction;

	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 ambientColor;
};

struct PointLight {
	glm::vec4 position;

	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 ambientColor;

	float linear, quadratic;
	// Needed for the amount of bytes to be a multiple of 16 (namely 80)
	float alignment1 = 0, alignment2 = 0;
};

struct SpotLight {
	glm::vec4 position;
	glm::vec4 direction;

	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 ambientColor;

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