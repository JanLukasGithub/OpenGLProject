#include "material.h"

Material::Material(const Material& mat) noexcept : diffuse{ mat.diffuse }, specular{ mat.specular }, emissive{ mat.emissive }, shininess{ mat.shininess }, diffuseMapName{ mat.diffuseMapName },
normalMapName{ mat.normalMapName }, diffuseMap{ mat.diffuseMap }, normalMap{ mat.normalMap } {}

Material::Material() noexcept : diffuse{ glm::vec3(0.0f) }, specular{ glm::vec3(0.0f) }, emissive{ glm::vec3(0.0f) }, shininess{ 0.0f }, diffuseMapName{ std::string() },
normalMapName{ std::string() }, diffuseMap{ 0 }, normalMap{ 0 } {}

bool Material::operator==(const Material& mat) {
    return mat.diffuse == this->diffuse && mat.specular == this->specular && mat.emissive == this->emissive && mat.shininess == this->shininess &&
    mat.diffuseMapName == this->diffuseMapName && mat.normalMapName == this->normalMapName;
}