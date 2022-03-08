#ifndef MODELS_MODEL_H_
#define MODELS_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../lib/stb_image.h"
#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/ext/matrix_transform.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "Mesh.h"
#include "../Shader.h"

class Model {
public:
	// Reads the model from filename and initializes it with shader
	Model(const char* filename, Shader* shader);
	// Reads the model from filename, initializes it with shader and adds the offset
	Model(const char* filename, Shader* shader, glm::vec3 offset);
	// Reads the model from filename, initializes it with shader and uses the modelMatrix
	Model(const char* filename, Shader* shader, glm::mat4 modelMat);

	// Translate the model
	void translate(glm::vec3 translation);
	// Rotate the model
	void rotate(glm::vec3 rotation);
	// Scale the model
	void scale(glm::vec3 scale);

	virtual ~Model();

	void render();
private:
	std::vector<Mesh*> m_meshes;
	std::vector<Material> m_materials;

	// Stores translation, rotation and scale
	glm::mat4 m_modelMat{0.0f};

	// Reads model from file using assimp
	void readModelFromFile(const char* filename, Shader* shader);
	// Processes the materials
	void processMaterials(const aiScene* scene, const char* path);
	// Processes the nodes recursively
	void processNodes(const aiScene* scene, aiNode* node, Shader* shader);
	// Processes the mesh
	void processMesh(aiMesh* mesh, Shader* shader);
};

#endif /* MODELS_MODEL_H_ */
