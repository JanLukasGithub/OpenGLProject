#ifndef _MODEL_FILE_H_
#define _MODEL_FILE_H_

#include <vector>
#include <algorithm>
#include <cstring>

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
    // Only allow access to the members of the vector. Does bounds checking
    static ModelFile& getFromList(int32 index);

private:
    // Name of this model
    const char* m_filename;
    // Meshes of this model file
    std::vector<Mesh*> m_meshes;
    // Holds pointers to the materials in the material list Material::materials, these are only valid until a new model is loaded
    std::vector<Material*> m_materials;

public:
    // Constructor
    ModelFile(const char* filename);
    // Deconstructor
    virtual ~ModelFile() noexcept;

    // Checks if the filename of model and filename are equal
    friend bool operator==(const ModelFile& model, const char* const filename);
    // Checks if the filename of model1 and filename of model2 are equal
    friend bool operator==(const ModelFile& model1, const ModelFile& model2);
};

#endif