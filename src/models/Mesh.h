/*
 * Mesh.h
 *
 *  Created on: Aug 15, 2021
 *      Author: jan
 */

class Mesh;

#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <assimp/types.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "../../lib/glm/glm.hpp"
#pragma GCC diagnostic pop

#include "../Shader.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../utils.h"

struct BMFMaterial {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float shininess;
};

struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float shininess;
	std::string diffuseMapName;
	std::string normalMapName;
	GLuint diffuseMap;
	GLuint normalMap;

public:
	Material(const Material& m) noexcept : diffuse{m.diffuse}, specular{m.specular}, emissive{m.emissive}, shininess{m.shininess}, diffuseMapName{m.diffuseMapName},
											normalMapName{m.normalMapName}, diffuseMap{m.diffuseMap}, normalMap{m.normalMap} {}

	Material() noexcept : diffuse{glm::vec3(0.0f)}, specular{glm::vec3(0.0f)}, emissive{glm::vec3(0.0f)}, shininess{0.0f}, diffuseMapName{std::string()},
							normalMapName{std::string()}, diffuseMap{0}, normalMap{0} {}
};

class Mesh {
public:
	Mesh(std::vector<Vertex>* vertices, std::vector<uint32>* indices, Material material, Shader* shader);
	virtual ~Mesh();

	// Renders this mesh
	void render();

	const std::vector<uint32>* getIndices() const {
		return m_indices;
	}

	const std::vector<Vertex>* getVertices() const {
		return m_vertices;
	}

private:
	// Currently not finished, DON'T USE
	Mesh& operator=(const Mesh& mesh);
	// Currently not finished, DON'T USE
	Mesh(const Mesh& mesh);

	// If there's a normal map
	bool m_hasNormalMap;

	uint64 m_numIndices;
	VertexBuffer* m_vbo;
	IndexBuffer* m_ibo;
	Shader* m_shader;
	Material m_material;
	// Just for memory cleanup
	std::vector<Vertex>* m_vertices;
	std::vector<uint32>* m_indices;
	// Uniforms
	int m_diffuseLocation;
	int m_specularLocation;
	int m_emissiveLocation;
	int m_shininessLocation;
	int m_diffuseMapLocation;
	int m_normalMapLocation;
	int m_hasNormalMapLocation;
};

#endif /* MESH_H_ */
