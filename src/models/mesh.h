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
	bool m_hasNormalMap;
	bool m_hasDiffuseMap;

	uint64 m_numIndices;
	VertexBuffer* m_vbo;
	IndexBuffer* m_ibo;
	int m_materialIndex;

public:
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32>& indices, int materialIndex);
	Mesh(Mesh&& mesh);
	virtual ~Mesh();

	// Renders this mesh
	void render() const;
	// Renders this mesh without setting vbo, ibo or anything. Use when rendering the same mesh multiple times
	void fastRender() const;

private:
	// Currently not finished, DON'T USE
	Mesh& operator=(const Mesh& mesh);
	// Currently not finished, DON'T USE
	Mesh(const Mesh& mesh);
};

#endif