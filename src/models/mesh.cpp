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

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<uint32>* indices, int materialIndex) : m_numIndices{ indices->size() },
m_vbo{ new VertexBuffer(vertices->data(), vertices->size()) }, m_ibo{ new IndexBuffer(indices->data(),
m_numIndices, sizeof(indices[0])) }, m_hasNormalMap{ Material::materials[materialIndex].normalMapName.compare("") != 0 }, m_materialIndex{ materialIndex },
m_hasDiffuseMap{ Material::materials[materialIndex].diffuseMapName.compare("") != 0 } {}

Mesh::Mesh(Mesh&& mesh) : m_numIndices{ mesh.m_numIndices }, m_vbo{ mesh.m_vbo }, m_ibo{ mesh.m_ibo }, m_hasNormalMap{ mesh.m_hasNormalMap },
m_materialIndex{ mesh.m_materialIndex }, m_hasDiffuseMap{ mesh.m_hasDiffuseMap } {
	mesh.m_vbo = nullptr;
	mesh.m_ibo = nullptr;
}

Mesh::~Mesh() {
	delete m_vbo;
	delete m_ibo;
}

void Mesh::render() const {
	Material material = Material::materials[m_materialIndex];

	m_vbo->bind();
	m_ibo->bind();
	glUniform1i(hasDiffuseMapLocation, m_hasDiffuseMap);
	glUniform1i(hasNormalMapLocation, m_hasNormalMap);
	glUniform3fv(diffuseLocation, 1, (float*)&material.diffuse);
	glUniform3fv(specularLocation, 1, (float*)&material.specular);
	glUniform3fv(emissiveLocation, 1, (float*)&material.emissive);
	glUniform1f(shininessLocation, material.shininess);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap);
	glUniform1i(diffuseMapLocation, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.normalMap);
	glUniform1i(normalMapLocation, 1);
	glActiveTexture(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::fastRender() const {
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
}

// UNFINISHED ---------------------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(const Mesh& mesh) : m_numIndices{ mesh.m_numIndices }, m_vbo{ new VertexBuffer(*mesh.m_vbo) }, m_ibo{ new IndexBuffer(*mesh.m_ibo) },
m_hasNormalMap{ mesh.m_hasNormalMap }, m_hasDiffuseMap{ mesh.m_hasDiffuseMap }, m_materialIndex{ mesh.m_materialIndex } {}

Mesh& Mesh::operator=(const Mesh& mesh) {
	if (this == &mesh)
		return *this;

	delete m_vbo;
	delete m_ibo;

	m_vbo = new VertexBuffer(*mesh.m_vbo);
	m_ibo = new IndexBuffer(*mesh.m_ibo);

	return *this;
}