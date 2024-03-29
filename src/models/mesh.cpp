#include "mesh.h"

void Mesh::initUniforms(Shader* shader) {
	diffuseLocation = glGetUniformLocation(shader->getShaderId(), "u_material.diffuse");
	specularLocation = glGetUniformLocation(shader->getShaderId(), "u_material.specular");
	emissiveLocation = glGetUniformLocation(shader->getShaderId(), "u_material.emissive");
	shininessLocation = glGetUniformLocation(shader->getShaderId(), "u_material.shininess");
	diffuseMapLocation = glGetUniformLocation(shader->getShaderId(), "u_diffuseMap");
	normalMapLocation = glGetUniformLocation(shader->getShaderId(), "u_normalMap");
	hasNormalMapLocation = glGetUniformLocation(shader->getShaderId(), "u_hasNormalMap");
	hasDiffuseMapLocation = glGetUniformLocation(shader->getShaderId(), "u_hasDiffuseMap");
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32>& indices, int materialIndex, const Model_Buffer& mbo) : m_numIndices{ indices.size() },
m_vao{ new Vertex_Array(vertices, mbo) }, m_ibo{ new IndexBuffer(indices.data(),
m_numIndices, sizeof(indices[0])) }, m_hasNormalMap{ Material_Manager::get_from_index(materialIndex).normal_map_index != 0 },
m_materialIndex{ materialIndex }, m_hasDiffuseMap{ Material_Manager::get_from_index(materialIndex).diffuse_map_index != 0 } {}

Mesh::Mesh(const Mesh& mesh) : m_numIndices{ mesh.m_numIndices }, m_vao{ new Vertex_Array(*mesh.m_vao) }, m_ibo{ new IndexBuffer(*mesh.m_ibo) }, 
m_hasNormalMap{ mesh.m_hasNormalMap }, m_materialIndex{ mesh.m_materialIndex }, m_hasDiffuseMap{ mesh.m_hasDiffuseMap } {}

Mesh::Mesh(Mesh&& mesh) : m_numIndices{ mesh.m_numIndices }, m_vao{ mesh.m_vao }, m_ibo{ mesh.m_ibo }, m_hasNormalMap{ mesh.m_hasNormalMap },
m_materialIndex{ mesh.m_materialIndex }, m_hasDiffuseMap{ mesh.m_hasDiffuseMap } {
	mesh.m_vao = nullptr;
	mesh.m_ibo = nullptr;
}

Mesh::~Mesh() {
	delete m_vao;
	delete m_ibo;
}

void Mesh::render(GLsizei num) const {
	Material material = Material_Manager::get_from_index(m_materialIndex);

	m_vao->bind();
	m_ibo->bind();
	glUniform1i(hasDiffuseMapLocation, m_hasDiffuseMap);
	glUniform1i(hasNormalMapLocation, m_hasNormalMap);
	glUniform3fv(diffuseLocation, 1, (float*)&material.diffuse);
	glUniform3fv(specularLocation, 1, (float*)&material.specular);
	glUniform3fv(emissiveLocation, 1, (float*)&material.emissive);
	glUniform1f(shininessLocation, material.shininess);
	Texture_Manager::get_from_index(material.diffuse_map_index).bind(GL_TEXTURE_2D, GL_TEXTURE0);
	glUniform1i(diffuseMapLocation, 0);
	Texture_Manager::get_from_index(material.normal_map_index).bind(GL_TEXTURE_2D, GL_TEXTURE1);
	glUniform1i(normalMapLocation, 1);

	glDrawElementsInstanced(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, num);
}