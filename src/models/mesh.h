class Mesh;

#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <GL/glew.h>

#include "../../lib/glm/glm.hpp"

#include "../shader.h"
#include "../vertexBuffer.h"
#include "../indexBuffer.h"
#include "../utils.h"
#include "material.h"

class Mesh {
public:
	Mesh(std::vector<Vertex>* vertices, std::vector<uint32>* indices, int materialIndex);
	virtual ~Mesh();

	// Renders this mesh
	void render();

	const std::vector<uint32>* getIndices() const {
		return m_indices;
	}

	const std::vector<Vertex>* getVertices() const {
		return m_vertices;
	}

	static void init(Shader* shader);

private:
	// Currently not finished, DON'T USE
	Mesh& operator=(const Mesh& mesh);
	// Currently not finished, DON'T USE
	Mesh(const Mesh& mesh);

	// If there's a diffuse/normal map
	bool m_hasNormalMap;
	bool m_hasDiffuseMap;

	uint64 m_numIndices;
	VertexBuffer* m_vbo;
	IndexBuffer* m_ibo;
	// Index of the material in the material list
	int m_materialIndex;
	// Just for memory cleanup
	std::vector<Vertex>* m_vertices;
	std::vector<uint32>* m_indices;
	// Uniforms
	inline static int m_diffuseLocation;
	inline static int m_specularLocation;
	inline static int m_emissiveLocation;
	inline static int m_shininessLocation;
	inline static int m_diffuseMapLocation;
	inline static int m_normalMapLocation;
	inline static int m_hasNormalMapLocation;
	inline static int m_hasDiffuseMapLocation;
};

#endif /* MESH_H_ */
