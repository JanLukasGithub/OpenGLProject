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
private:
	// Uniforms
	inline static int diffuseLocation;
	inline static int specularLocation;
	inline static int emissiveLocation;
	inline static int shininessLocation;
	inline static int diffuseMapLocation;
	inline static int normalMapLocation;
	inline static int hasNormalMapLocation;
	inline static int hasDiffuseMapLocation;
public:
	static void initUniforms(Shader* shader);

private:
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

public:
	Mesh(std::vector<Vertex>* vertices, std::vector<uint32>* indices, int materialIndex);
	virtual ~Mesh();

	// Renders this mesh
	void render();

	const std::vector<uint32>* getIndices() const noexcept;

	const std::vector<Vertex>* getVertices() const noexcept;

private:
	// Currently not finished, DON'T USE
	Mesh& operator=(const Mesh& mesh);
	// Currently not finished, DON'T USE
	Mesh(const Mesh& mesh);
};

#endif /* MESH_H_ */
