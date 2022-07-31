#ifndef LIGHTS_LIGHTS_H_
#define LIGHTS_LIGHTS_H_

#include <vector>

#include "../lib/glm/glm.hpp"

#include "shader.h"
#include "utils.h"
#include "buffer/shaderBuffer.h"

#define BUFFER_BINDING_DIRECTIONAL_LIGHT 0
#define BUFFER_BINDING_POINT_LIGHT 1
#define BUFFER_BINDING_SPOT_LIGHT 2

struct DirectionalLight {
	glm::vec3 direction;
	float alignment1 = 0;

	glm::vec3 diffuseColor;
	float alignment2 = 0;
	glm::vec3 specularColor;
	float alignment3 = 0;
	glm::vec3 ambientColor;
	float alignment4 = 0;
};

struct PointLight {
	glm::vec3 position;
	float linear;
	glm::vec3 diffuseColor;
	float quadratic;
	glm::vec3 specularColor;
	float alignment1 = 0;
	glm::vec3 ambientColor;
	float alignment2 = 0;
};

struct SpotLight {
	glm::vec3 position;
	float linear;
	glm::vec3 direction;
	float quadratic;
	glm::vec3 diffuseColor;
	float innerCone;
	glm::vec3 specularColor;
	float outerCone;
	glm::vec3 ambientColor;
	float alignment1 = 0.0f;
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
