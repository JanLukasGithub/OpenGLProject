class Model;

#ifndef MODELS_MODEL_H_
#define MODELS_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>

#include "../../lib/stb_image.h"
#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "mesh.h"
#include "material.h"
#include "modelFile.h"
#include "../shader.h"
#include "../utils.h"

class Model {
public:
	// Location of model mat uniform
	inline static int modelMatLocation;
	static void initUniforms(Shader* shader);

private:
	// Stores translation, rotation and scale
	glm::mat4 m_modelMat{ 1.0f };

public:
	// Reads the model from filename and initializes it with shader
	Model(const char* filename);
	// Reads the model from filename, initializes it with shader and adds the offset
	Model(const char* filename, glm::vec3 position);
	// Reads the model from filename, initializes it with shader and uses the modelMatrix
	Model(const char* filename, glm::mat4 modelMat);
	// Initializes the model from the ModelFile
	Model(ModelFile& modelFile);
	// Initializes the model from the ModelFile and places it at the position
	Model(ModelFile& modelFile, glm::vec3 position);
	// Initializes the model from the ModelFile and uses the modelMatrix
	Model(ModelFile& modelFile, glm::mat4 modelMat);
	// Deletes meshes
	virtual ~Model() {};

	// Translate the model
	void translate(glm::vec3 translation);
	// Rotate the model, with rotation in radians
	void rotate(glm::vec3 rotation);
	// Scale the model
	void scale(glm::vec3 scale);

	glm::mat4& getModelMat() noexcept;
};

#endif /* MODELS_MODEL_H_ */
