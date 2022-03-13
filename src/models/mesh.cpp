#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<uint32>* indices, int materialIndex) : m_numIndices{ indices->size() },
m_vbo{ new VertexBuffer(vertices->data(), vertices->size(), &m_hasNormalMap) }, m_ibo{ new IndexBuffer(indices->data(),
m_numIndices, sizeof(indices[0])) }, m_vertices{ vertices }, m_indices{ indices },
m_hasNormalMap{ Material::materials[materialIndex].normalMapName.compare("") != 0 }, m_materialIndex{ materialIndex },
m_hasDiffuseMap{ Material::materials[materialIndex].diffuseMapName.compare("") != 0 } {}


Mesh::Mesh(const Mesh& mesh) : m_numIndices{ mesh.m_numIndices }, m_vbo{ new VertexBuffer(*mesh.m_vbo) }, m_ibo{ new IndexBuffer(*mesh.m_ibo) },
m_vertices{ new std::vector<Vertex>(*mesh.m_vertices) }, m_indices{ new std::vector<uint32>(*mesh.m_indices) },
m_hasNormalMap{ mesh.m_hasNormalMap }, m_hasDiffuseMap{ mesh.m_hasDiffuseMap }, m_materialIndex{ mesh.m_materialIndex } {}

Mesh::~Mesh() {
	delete m_indices;
	delete m_vertices;

	delete m_vbo;
	delete m_ibo;
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	// self-assignment check
	if (this == &mesh)
		return *this;

	// Delete old values
	if (m_indices)
		delete m_indices;
	if (m_vertices)
		delete m_vertices;

	if (m_vbo)
		delete m_vbo;
	if (m_ibo)
		delete m_ibo;

	// Initialize new values
	m_indices = new std::vector<uint32>(*mesh.m_indices);
	m_vertices = new std::vector<Vertex>(*mesh.m_vertices);

	m_vbo = new VertexBuffer(*mesh.m_vbo);
	m_ibo = new IndexBuffer(*mesh.m_ibo);

	// Return this object
	return *this;
}

void Mesh::render() {
	Material material = Material::materials[m_materialIndex];

	m_vbo->bind();
	m_ibo->bind();
	glUniform1i(m_hasDiffuseMapLocation, m_hasDiffuseMap);
	glUniform1i(m_hasNormalMapLocation, m_hasNormalMap);
	glUniform3fv(m_diffuseLocation, 1, (float*)&material.diffuse);
	glUniform3fv(m_specularLocation, 1, (float*)&material.specular);
	glUniform3fv(m_emissiveLocation, 1, (float*)&material.emissive);
	glUniform1f(m_shininessLocation, material.shininess);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap);
	glUniform1i(m_diffuseMapLocation, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.normalMap);
	glUniform1i(m_normalMapLocation, 1);
	glActiveTexture(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	m_ibo->unbind();
	m_vbo->unbind();
}

void Mesh::init(Shader* shader) {
	m_diffuseLocation = glGetUniformLocation(shader->getShaderId(), "u_material.diffuse");
	m_specularLocation = glGetUniformLocation(shader->getShaderId(), "u_material.specular");
	m_emissiveLocation = glGetUniformLocation(shader->getShaderId(), "u_material.emissive");
	m_shininessLocation = glGetUniformLocation(shader->getShaderId(), "u_material.shininess");
	m_diffuseMapLocation = glGetUniformLocation(shader->getShaderId(), "u_diffuseMap");
	m_normalMapLocation = glGetUniformLocation(shader->getShaderId(), "u_normalMap");
	m_hasNormalMapLocation = glGetUniformLocation(shader->getShaderId(), "u_hasNormalMap");
	m_hasDiffuseMapLocation = glGetUniformLocation(shader->getShaderId(), "u_hasDiffuseMap");
}
