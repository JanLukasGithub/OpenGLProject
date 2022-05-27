#ifndef MODEL_UTILS_H_
#define MODEL_UTILS_H_

#include <filesystem>
#include <string>

#include "model.h"

namespace utils {
// Loads models from the specified path recursively
void loadModelsFrom(const std::string& path) noexcept;

// Loads the model from the specified filename. Handles exceptions by telling the user in std::cerr
// Returns true if model instance was loaded successfully, false otherwise
bool loadModelInstance(const std::string& filename, const glm::vec3 position) noexcept;
}

#endif