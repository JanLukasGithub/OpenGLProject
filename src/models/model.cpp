#include "model.h"

void Model::initUniforms(Shader* shader) {
	Model::modelMatLocation = glGetUniformLocation(shader->getShaderId(), "u_modelMat");
}

Model::Model(const char* filename) : m_meshesIndex{ ModelFile::addModelFile(filename) } {}

Model::Model(const char* filename, glm::vec3 offset) : m_meshesIndex{ ModelFile::addModelFile(filename) } {
	m_modelMat = glm::translate(m_modelMat, offset);
}

Model::Model(const char* filename, glm::mat4 modelMat) : m_meshesIndex{ ModelFile::addModelFile(filename) }, m_modelMat{ modelMat } {}

Model::~Model() {}

void Model::translate(glm::vec3 translation) {
	m_modelMat = glm::translate(m_modelMat, translation);
}

void Model::rotate(glm::vec3 rotation) {
	m_modelMat = glm::rotate(m_modelMat, rotation.x, glm::vec3(1, 0, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.y, glm::vec3(0, 1, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.z, glm::vec3(0, 0, 1));
}

void Model::scale(glm::vec3 scale) {
	m_modelMat = glm::scale(m_modelMat, scale);
}

void Model::render() {
	glUniformMatrix4fv(Model::modelMatLocation, 1, GL_FALSE, &m_modelMat[0][0]);

	std::vector<Mesh*>& meshes = ModelFile::getFromList(m_meshesIndex).getMeshes();
	for (uint32 i = 0; i < meshes.size(); i++) {
		meshes[i]->render();
	}
}