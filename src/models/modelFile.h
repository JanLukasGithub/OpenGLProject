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

// Representation of a model file (.obj or similar) in code. Allows you to create Models
class ModelFile {
private:
    inline static std::vector<ModelFile> modelFiles{};
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
    static int32 indexOf(const char* filename);

    /**
     * @brief Get the index of the specified modelFile
     *
     * @return the index of the specified model or -1 if none was found
     */
    static int32 indexOf(const ModelFile& model);

    // Only allow access to the members of the vector, not the vector itself. Does bounds checking
    static ModelFile& getFromList(int32 index);

private:
    // Name of this model
    const char* m_filename;
    // Meshes of this model file
    std::vector<Mesh*> m_meshes{};
    // Holds the indices of the materials
    std::vector<uint32> m_materialIndices{};

public:
    // Move constructor
    ModelFile(ModelFile&& model);
    // Deconstructor
    virtual ~ModelFile() noexcept;

    // Checks if the filename of model and filename are equal
    friend bool operator==(const ModelFile& model, const char* const filename);
    // Checks if the filename of model1 and filename of model2 are equal
    friend bool operator==(const ModelFile& model1, const ModelFile& model2);

private:
    // Private constructor to force use of addModelFile(filename)
    ModelFile(const char* filename);
    // Private copy constructor to prevent use of it
    ModelFile(const ModelFile& model) {};
    // Private assignment operator to prevent use of it
    ModelFile& operator=(const ModelFile& model) { return *this; }

    // Reads model from file using assimp
    void readModelFromFile(const char* filename);
    // Processes the materials, returns the number of materials loaded
    void processMaterials(const aiScene* scene, const char* path);
    // Processes the nodes recursively
    void processNodes(const aiScene* scene, aiNode* node);
    // Processes the mesh
    void processMesh(aiMesh* mesh);
};

#endif