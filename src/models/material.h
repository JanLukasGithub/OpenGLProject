struct Material;

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>
#include <string>

#include "../../lib/glm/glm.hpp"

struct Material {
public:
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float shininess;
	std::string diffuseMapName;
	std::string normalMapName;
	GLuint diffuseMap;
	GLuint normalMap;

	Material(const Material& m) noexcept : diffuse{ m.diffuse }, specular{ m.specular }, emissive{ m.emissive }, shininess{ m.shininess }, diffuseMapName{ m.diffuseMapName },
		normalMapName{ m.normalMapName }, diffuseMap{ m.diffuseMap }, normalMap{ m.normalMap } {}

	Material() noexcept : diffuse{ glm::vec3(0.0f) }, specular{ glm::vec3(0.0f) }, emissive{ glm::vec3(0.0f) }, shininess{ 0.0f }, diffuseMapName{ std::string() },
		normalMapName{ std::string() }, diffuseMap{ 0 }, normalMap{ 0 } {}
};

#endif