#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>

#include "../../lib/glm/glm.hpp"

struct Material {
	// List of all materials loaded
	inline static std::vector<Material> materials{};

	glm::vec3 diffuse{ 0 };
	glm::vec3 specular{ 0 };
	glm::vec3 emissive{ 0 };
	float shininess{ 0.0f };
	uint32 diffuse_map_index{ 0 };
	uint32 normal_map_index{ 0 };

	bool operator==(const Material&) const = default;
};

#endif