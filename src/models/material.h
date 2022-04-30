#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>

#include "../../lib/glm/glm.hpp"

struct Material {
public:
	// List of all materials loaded
	inline static std::vector<Material> materials{};

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float shininess;
	std::string diffuseMapName;
	std::string normalMapName;
	GLuint diffuseMap;
	GLuint normalMap;

	Material(const Material& mat) noexcept;
	Material() noexcept;

	/**
	 * @brief compares this material to another one ignoring the 'diffuseMap' and 'normalMap' members, but not 'diffuseMapName' and 'normalMapName'
	 * 
	 * @return true if all members except for 'diffuseMap' and 'normalMap' are equal, false otherwise
	 */
	bool operator==(const Material& mat);
};

#endif