#include "materialManager.h"

static std::vector<Material> materials{};

uint32 Material_Manager::get_material_index(aiMaterial* mat, const std::string& model_filename) {
    Material m{ mat, model_filename };

    auto it = std::find(materials.begin(), materials.end(), m);

    if (it != materials.end())
        return it - materials.begin();
    else {
        materials.push_back(m);

        return materials.size() - 1;
    }
}

const Material& Material_Manager::get_from_index(uint32 index) {
    return materials[index];
}
