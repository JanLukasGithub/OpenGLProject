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
#include "../shader.h"

class Model {
private:
	// Location of model mat uniform
	inline static int m_modelMatLocation;
public:
	static void initUniforms(Shader* shader);

private:
	std::vector<Mesh*> m_meshes;
	// Holds pointers to the materials in the material list Material::materials, these are only valid until a new model is loaded
	std::vector<Material*> m_materials;
	// Stores translation, rotation and scale
	glm::mat4 m_modelMat{ 1.0f };

public:
	// Reads the model from filename and initializes it with shader
	Model(const char* filename);
	// Reads the model from filename, initializes it with shader and adds the offset
	Model(const char* filename, glm::vec3 offset);
	// Reads the model from filename, initializes it with shader and uses the modelMatrix
	Model(const char* filename, glm::mat4 modelMat);
	// Deletes meshes
	virtual ~Model();

	// Translate the model
	void translate(glm::vec3 translation);
	// Rotate the model, with rotation in radians
	void rotate(glm::vec3 rotation);
	// Scale the model
	void scale(glm::vec3 scale);
	// Render the model
	void render();

private:
	// Reads model from file using assimp
	void readModelFromFile(const char* filename);
	// Processes the materials, returns the number of materials loaded
	void processMaterials(const aiScene* scene, const char* path);
	// Processes the nodes recursively
	void processNodes(const aiScene* scene, aiNode* node);
	// Processes the mesh
	void processMesh(aiMesh* mesh);
};

#endif /* MODELS_MODEL_H_ */
