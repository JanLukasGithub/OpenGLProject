#include "modelInstance.h"

void ModelInstance::initUniforms(Shader* shader) {
	ModelInstance::modelMatLocation = glGetUniformLocation(shader->getShaderId(), "u_modelMat");
}

ModelInstance::ModelInstance(const char* filename) {
	Model::getFromList(Model::addModelFile(filename)).addModel(this);
}

ModelInstance::ModelInstance(const char* filename, glm::vec3 position) : m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	Model::getFromList(Model::addModelFile(filename)).addModel(this);
}

ModelInstance::ModelInstance(const char* filename, glm::mat4 modelMat) : m_modelMat{ modelMat } {
	Model::getFromList(Model::addModelFile(filename)).addModel(this);
}

ModelInstance::ModelInstance(Model& modelFile) {
	modelFile.addModel(this);
}

ModelInstance::ModelInstance(Model& modelFile, glm::vec3 position) : m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	modelFile.addModel(this);
}

ModelInstance::ModelInstance(Model& modelFile, glm::mat4 modelMat) : m_modelMat{ modelMat } {
	modelFile.addModel(this);
}

void ModelInstance::translate(glm::vec3 translation) {
	m_modelMat = glm::translate(m_modelMat, translation);
}

void ModelInstance::rotate(glm::vec3 rotation) {
	m_modelMat = glm::rotate(m_modelMat, rotation.x, glm::vec3(1, 0, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.y, glm::vec3(0, 1, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.z, glm::vec3(0, 0, 1));
}

void ModelInstance::scale(glm::vec3 scale) {
	m_modelMat = glm::scale(m_modelMat, scale);
}

glm::mat4& ModelInstance::getModelMat() noexcept {
	return m_modelMat;
}