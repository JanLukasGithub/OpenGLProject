#include "model.h"

void Model::initUniforms(Shader* shader) {
	Model::modelMatLocation = glGetUniformLocation(shader->getShaderId(), "u_modelMat");
}

Model::Model(const char* filename) : m_meshesIndex{ ModelFile::addModelFile(filename) } {
	ModelFile::getFromList(m_meshesIndex).addModel(this);
}

Model::Model(const char* filename, glm::vec3 position) : m_meshesIndex{ ModelFile::addModelFile(filename) }, m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	ModelFile::getFromList(m_meshesIndex).addModel(this);
}

Model::Model(const char* filename, glm::mat4 modelMat) : m_meshesIndex{ ModelFile::addModelFile(filename) }, m_modelMat{ modelMat } {
	ModelFile::getFromList(m_meshesIndex).addModel(this);
}

Model::Model(ModelFile& modelFile) : m_meshesIndex{ ModelFile::indexOf(modelFile) } {
	modelFile.addModel(this);
}

Model::Model(ModelFile& modelFile, glm::vec3 position) : m_meshesIndex{ ModelFile::indexOf(modelFile) }, m_modelMat{ glm::translate(glm::mat4(1.0f), position) } {
	modelFile.addModel(this);
}

Model::Model(ModelFile& modelFile, glm::mat4 modelMat) : m_meshesIndex{ ModelFile::indexOf(modelFile) }, m_modelMat{ modelMat } {
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

void Model::render() const {
	glUniformMatrix4fv(Model::modelMatLocation, 1, GL_FALSE, &m_modelMat[0][0]);

	std::vector<Mesh*>& meshes = ModelFile::getFromList(m_meshesIndex).getMeshes();
	for (uint32 i = 0; i < meshes.size(); i++) {
		meshes[i]->render();
	}
}

std::strong_ordering operator<=>(const Model& model1, const Model& model2) noexcept {
	int model1Size = ModelFile::getFromList(model1.m_meshesIndex).getMeshes().size();
	int model2Size = ModelFile::getFromList(model2.m_meshesIndex).getMeshes().size();

	if ((model1Size <=> model2Size) == std::strong_ordering::equal) {
		return model1.m_meshesIndex <=> model2.m_meshesIndex;
	}

	return model1Size <=> model2Size;
}

int Model::getMeshIndex() const noexcept {
	return m_meshesIndex;
}

glm::mat4& Model::getModelMat() noexcept {
	return m_modelMat;
}