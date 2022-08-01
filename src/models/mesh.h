#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <GL/glew.h>

#include "../../lib/glm/glm.hpp"

#include "../shader.h"
#include "../buffer/vertexArray.h"
#include "../buffer/vertexBuffer.h"
#include "../buffer/indexBuffer.h"
#include "../utils.h"
#include "../texture/textureManager.h"
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
	Vertex_Array* m_vao;
	IndexBuffer* m_ibo;
	int m_materialIndex;

public:
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32>& indices, int materialIndex, const Model_Buffer& mbo);
	Mesh(const Mesh& mesh);
	Mesh(Mesh&& mesh);
	virtual ~Mesh();

	// Renders this mesh num times
	void render(GLsizei num) const;
};

#endif