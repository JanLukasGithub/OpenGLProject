#ifndef MODEL_UTILS_H_
#define MODEL_UTILS_H_

#include <filesystem>

#include "model.h"

namespace utils {
// Loads models from the specified path recursively
void loadModelsFrom(const char* const path) noexcept;

// Loads the model from the specified filename. Handles exceptions by telling the user in std::cerr
// Returns true iff model instance was loaded successfully, false otherwise
bool loadModelInstance(const char* const filename, const glm::vec3 position) noexcept;
}

#endif