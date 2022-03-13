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

	Material(const Material& m) noexcept;
	Material() noexcept;
};

#endif