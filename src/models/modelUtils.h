#ifndef MODEL_UTILS_H_
#define MODEL_UTILS_H_

#include <filesystem>

#include "model.h"

namespace utils {
// Loads models from the specified path recursively
void loadModelsFrom(const char* const path) noexcept;
}

#endif