#ifndef _MODEL_FILE_H_
#define _MODEL_FILE_H_

#include <vector>
#include <algorithm>

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
    // Seperate indexOf(filename) function to avoid confusion or accidental model loading. Returns -1 if no model was found
    static int32 indexOf(const char* filename);
    // Only allow access to the members of the vector
    static ModelFile& getFromList(int32 index);

private:
    // Name of this model
    const char* filename;
    // Meshes of this model file
    std::vector<Mesh*> m_meshes;
    // Holds pointers to the materials in the material list Material::materials, these are only valid until a new model is loaded
    std::vector<Material*> m_materials;

};

#endif