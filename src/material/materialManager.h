#ifndef MATERIAL_MANAGER_H_
#define MATERIAL_MANAGER_H_

#include <string>

#include <assimp/scene.h>

#include "material.h"

namespace Material_Manager {
    uint32 get_material_index(aiMaterial* mat, const std::string& model_filename);
    const Material& get_from_index(uint32 index);
}

#endif