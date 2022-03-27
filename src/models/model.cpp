#include "model.h"

void Model::initUniforms(Shader* shader) {
	Model::modelMatLocation = glGetUniformLocation(shader->getShaderId(), "u_modelMat");
}

Model::Model(const char* filename) {
	ModelFile::getFromList(ModelFile::addModelFile(filename)).addModel(this);
}

Model::Model(const char* filename, glm::vec3 position) : m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	ModelFile::getFromList(ModelFile::addModelFile(filename)).addModel(this);
}

Model::Model(const char* filename, glm::mat4 modelMat) : m_modelMat{ modelMat } {
	ModelFile::getFromList(ModelFile::addModelFile(filename)).addModel(this);
}

Model::Model(ModelFile& modelFile) {
	modelFile.addModel(this);
}

Model::Model(ModelFile& modelFile, glm::vec3 position) : m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	modelFile.addModel(this);
}

Model::Model(ModelFile& modelFile, glm::mat4 modelMat) : m_modelMat{ modelMat } {
	modelFile.addModel(this);
}

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

glm::mat4& Model::getModelMat() noexcept {
	return m_modelMat;
}