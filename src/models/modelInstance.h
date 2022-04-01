class ModelInstance;

#ifndef MODELS_MODEL_H_
#define MODELS_MODEL_H_

#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../shader.h"

class ModelInstance {
public:
	// Location of model mat uniform
	inline static int modelMatLocation;
	static void initUniforms(const Shader* shader);

private:
	// Stores translation, rotation and scale
	glm::mat4 m_modelMat{ 1.0f };

public:
	// Model at position 0, 0, 0
	ModelInstance();
	// Model at specified position
	ModelInstance(const glm::vec3 position);
	// Model using the specified modelMat
	ModelInstance(const glm::mat4 modelMat);
	// Deletes meshes
	virtual ~ModelInstance() {};

	// Translate the model
	void translate(const glm::vec3 translation);
	// Rotate the model, with rotation in radians
	void rotate(const glm::vec3 rotation);
	// Scale the model
	void scale(const glm::vec3 scale);

	const glm::mat4& getModelMat() noexcept;
};

#endif /* MODELS_MODEL_H_ */
