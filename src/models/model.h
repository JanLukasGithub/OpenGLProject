class Model;

#ifndef _MODEL_FILE_H_
#define _MODEL_FILE_H_

#include <vector>
#include <algorithm>
#include <cstring>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../lib/stb_image.h"
#include "../../lib/glm/glm.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"

#include "../utils.h"
#include "mesh.h"
#include "material.h"
#include "modelInstance.h"

// Representation of a model file (.obj or similar) in code. Allows you to create Models
class Model {
private:
    inline static std::vector<Model> modelFiles{};
public:
    /**
     * @brief Adds the model from the specified path to the list. If the model is already in the list this does nothing. Can be used as indexOf(filename)
     * while also loading the model if it wasn't loaded before
     *
     * @param filename
     * @return the index of the model with the specified filename
     */
    static int32 addModelFile(const char* filename);

    /**
     * @brief Get the index of the modelFile with the specified filename. addModelFile(filename) can do this as well,
     * but using that function might be confusing for getting the index, and that function has the side effect of loading a new model if none was found
     *
     * @return the index of the model with the specified filename or -1 if none was found
     */
    static int32 indexOf(const char* const filename);

    /**
     * @brief Get the index of the specified modelFile
     *
     * @return the index of the specified model or -1 if none was found
     */
    static int32 indexOf(const Model& model);

    // Only allow access to the members of the vector, not the vector itself. Doesn't do bounds checking
    static Model& getFromList(int32 index);

    /**
     * @brief Get the model from the specified filename
     * 
     * @return the model with the specified filename or nullptr if none exists
     */
    static Model* getFromList(const char* const filename);

    // Return the size of the list
    static int getListSize();

private:
    // Name of this model
    const char* m_filename;
    // Meshes of this model file
    std::vector<Mesh*> m_meshes{};
    // Holds the indices of the materials
    std::vector<uint32> m_materialIndices{};
    // Holds the Models made from this Model
    std::vector<ModelInstance*> m_models{};

public:
    // Move constructor
    Model(Model&& model);
    // Deconstructor
    virtual ~Model() noexcept;

    // Adds the model to the list of models with this Model
    void addModel(ModelInstance* model) noexcept;

    // Renders all models in the list
    void renderModels() noexcept;

    // Checks if the filename of model and filename are equal
    friend bool operator==(const Model& model, const char* const filename);
    // Checks if the filename of model1 and filename of model2 are equal
    friend bool operator==(const Model& model1, const Model& model2);

private:
    // Private constructor to force use of addModelFile(filename)
    Model(const char* filename);
    // Private copy constructor to prevent use of it
    Model(const Model& model) {};
    // Private copy assignment operator to prevent use of it
    Model& operator=(const Model& model) { return *this; }

    // Reads model from file using assimp
    void readModelFromFile();
    // Processes the materials, returns the number of materials loaded
    void processMaterials(const aiScene* scene);
    // Processes the nodes recursively
    void processNodes(const aiScene* scene, aiNode* node);
    // Processes the mesh
    void processMesh(aiMesh* mesh);

public:
    std::vector<Mesh*>& getMeshes() { return m_meshes; }
};

#endif