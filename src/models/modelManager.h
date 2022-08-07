#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

#include <string>

#include "../utils.h"
#include "model.h"

namespace Model_Manager {
    uint32 get_model_index(const std::string& filename);
    Model& get_from_index(uint32 index);
    uint32 get_num_models();
}

#endif