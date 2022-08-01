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
#include "material.h"
#include "../texture/textureManager.h"
#include "../renderer/IRenderable.h"
#include "../buffer/shaderBuffer.h"
#include "../buffer/modelBuffer.h"

// Representation of a model file (.obj or similar) in code. Allows you to create Models
class Model : IRenderable {
private:
    inline static std::vector<Model> modelFiles{};
public:
    /**
     * @brief Adds the model from the specified path to the list. If the model is already in the list this does nothing. Can be used as indexOf(filename)
     * while also loading the model if it wasn't loaded before
     *
     * @param filename
     * @return a reference to the model added. Is invalidated when another one is added
     */
    static Model& addModelFile(const std::string& filename);

    /**
     * @brief Get the index of the modelFile with the specified filename. addModelFile(filename) can do this as well,
     * but using that function might be confusing for getting the index, and that function has the side effect of loading a new model if none was found
     *
     * @return the index of the model with the specified filename or -1 if none was found
     */
    static const int32 indexOf(const std::string& filename) noexcept;

    /**
     * @brief Get the index of the specified modelFile
     *
     * @return the index of the specified model or -1 if none was found
     */
    static const int32 indexOf(const Model& model) noexcept;

    // Only allow access to the members of the vector, not the vector itself. Doesn't do bounds checking
    static Model& getFromList(const int32 index) noexcept;

    /**
     * @brief Get the model from the specified filename
     *
     * @return the model with the specified filename or nullptr if none exists. Is invalidated when a model is added
     */
    static Model* getFromList(const std::string& filename) noexcept;

    // Return the size of the list
    static const int getListSize() noexcept;

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

    // Renders all models in the list
    void renderModels() const noexcept;
    virtual void render() const noexcept override;

    // Checks if the filename of model and filename are equal
    friend bool operator==(const Model& model, const std::string& filename);
    // Checks if the filename of model1 and filename of model2 are equal
    friend bool operator==(const Model& model1, const Model& model2);

private:
    // Private constructor to force use of addModelFile(filename)
    Model(const std::string& filename);

    // Reads model from file using assimp
    void readModelFromFile();
    // Processes the materials
    void processMaterials(const aiScene* scene);
    // Gets a vec3 from the aiMaterial, most commonly a color. Use AI_MATKEY_COLOR_XXX for the 3 last arguments
    glm::vec3 getColor(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx);
    // Gets a single float from the aiMaterial. Use AI_MATKEY_COLOR_XXX for the 3 last arguments
    float getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx);
    // Gets a tetxure from the aiMaterial
    std::string getTexturePath(aiMaterial* mat, aiTextureType type);
    // Processes the nodes recursively
    void processNodes(const aiScene* scene, aiNode* node);
    // Processes the mesh
    void processMesh(aiMesh* mesh);

public:
    std::vector<Mesh>& getMeshes() { return m_meshes; }
};

#endif