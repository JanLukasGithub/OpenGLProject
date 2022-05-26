#include "modelInstance.h"

ModelInstance::ModelInstance() {}

ModelInstance::ModelInstance(const glm::vec3 position) : m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {}

ModelInstance::ModelInstance(const glm::mat4 modelMat) : m_modelMat{ modelMat } {}

void ModelInstance::translate(const glm::vec3 translation) {
	m_modelMat = glm::translate(m_modelMat, translation);
}

void ModelInstance::rotate(const glm::vec3 rotation) {
	m_modelMat = glm::rotate(m_modelMat, rotation.x, glm::vec3(1, 0, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.y, glm::vec3(0, 1, 0));
	m_modelMat = glm::rotate(m_modelMat, rotation.z, glm::vec3(0, 0, 1));
}

void ModelInstance::scale(const glm::vec3 scale) {
	m_modelMat = glm::scale(m_modelMat, scale);
}

const glm::mat4& ModelInstance::getModelMat() noexcept {
	return m_modelMat;
}