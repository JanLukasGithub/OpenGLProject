#ifndef _MODEL_FILE_H_
#define _MODEL_FILE_H_

#include <vector>
#include <algorithm>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../utils.h"
#include "mesh.h"
#include "../material/materialManager.h"
#include "../texture/textureManager.h"
#include "../renderer/IRenderable.h"
#include "../buffer/shaderBuffer.h"
#include "../buffer/modelBuffer.h"

// Representation of a model file (.obj or similar) in code. Allows you to create Models
class Model : IRenderable {
private:
    // Name of this model
    const std::string m_filename;
    // Meshes of this model file
    std::vector<Mesh> m_meshes{};
    // Holds the indices of the materials
    std::vector<uint32> m_materialIndices{};
    // Buffer to store model mats to
    Model_Buffer m_model_mat_buffer;

public:
    Model(const std::string& filename);
    // Move constructor
    Model(Model&& model);
    // Deconstructor
    virtual ~Model() noexcept;

    // Adds an instance
    GLsizeiptr addInstance() noexcept;
    // Adds an instance at the specified position
    GLsizeiptr addInstance(const glm::vec3 position) noexcept;
    // Adds an instance using the specified model matrix
    GLsizeiptr addInstance(const glm::mat4 modelMat) noexcept;

    // Set an instance's model mat
    void set_instance(const GLsizeiptr index, const glm::mat4& model_mat);

    // Renders all models in the list
    void renderModels() const noexcept;
    virtual void render() const noexcept override;

    // Checks if the filename of model and filename are equal
    friend bool operator==(const Model& model, const std::string& filename);
    // Checks if the filename of model1 and filename of model2 are equal
    friend bool operator==(const Model& model1, const Model& model2);

private:
    // Reads model from file using assimp
    void readModelFromFile();
    // Processes the materials
    void processMaterials(const aiScene* scene);
    // Processes the nodes recursively
    void processNodes(const aiScene* scene, aiNode* node);
    // Processes the mesh
    void processMesh(aiMesh* mesh);

public:
    std::vector<Mesh>& getMeshes() { return m_meshes; }
};

#endif