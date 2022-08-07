#include "modelManager.h"

#include <vector>

static std::vector<Model> models{};

uint32 Model_Manager::get_model_index(const std::string& filename) {
    auto it = std::find(models.begin(), models.end(), filename);

    if (it != models.end())
        return it - models.begin();
    else {
        models.push_back(Model(filename));

        return models.size() - 1;
    }
}

Model& Model_Manager::get_from_index(uint32 index) {
    return models[index];
}

uint32 Model_Manager::get_num_models() {
    return models.size();
}
