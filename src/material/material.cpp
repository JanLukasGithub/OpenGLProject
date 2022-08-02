#include "material.h"

glm::vec3 getColor(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx) {
    aiColor3D color(0.0f, 0.0f, 0.0f);
    if (AI_SUCCESS != mat->Get(pKey, type, idx, color)) {
        debugOutputEndl("No color found, using default color black!");
    }
    return glm::vec3(color.r, color.g, color.b);
}

float getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx) {
    float value(0.0f);
    if (AI_SUCCESS != mat->Get(pKey, type, idx, value)) {
        debugOutputEndl("No value, using default value 0!");
    }
    return value;
}

std::string getTexturePath(aiMaterial* mat, aiTextureType type, const std::string& model_filename) {
    if (mat->GetTextureCount(type) > 0) {
        aiString textureNameBuffer{ };
        mat->GetTexture(type, 0, &textureNameBuffer);
        return getFilePath(model_filename) + textureNameBuffer.C_Str();
    }

    debugOutputEndl("No texture found, using default texture!");
    return std::string("");
}

Material::Material() {}

Material::Material(aiMaterial* mat, const std::string& model_filename) :
diffuse{ getColor(mat, AI_MATKEY_COLOR_DIFFUSE) },
specular{ getColor(mat, AI_MATKEY_COLOR_SPECULAR) * getFloat(mat, AI_MATKEY_SHININESS_STRENGTH) },
emissive{ getColor(mat, AI_MATKEY_COLOR_EMISSIVE) },
shininess{ getFloat(mat, AI_MATKEY_SHININESS) },
diffuse_map_index{ Texture_Manager::get_texture_index(getTexturePath(mat, aiTextureType_DIFFUSE, model_filename)) },
normal_map_index{ Texture_Manager::get_texture_index(getTexturePath(mat, aiTextureType_NORMALS, model_filename)) } {}
