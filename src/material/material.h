#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>

#include <assimp/scene.h>

#include "../../lib/glm/glm.hpp"

#include "../utils.h"
#include "../texture/textureManager.h"

struct Material {
	// List of all materials loaded
	inline static std::vector<Material> materials{};

	Material();
	Material(aiMaterial* mat, const std::string& model_filename);

	glm::vec3 diffuse{ 0 };
	glm::vec3 specular{ 0 };
	glm::vec3 emissive{ 0 };
	float shininess{ 0.0f };
	uint32 diffuse_map_index{ 0 };
	uint32 normal_map_index{ 0 };

	bool operator==(const Material&) const noexcept = default;
};

#endif