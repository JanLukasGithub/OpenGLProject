#include "modelUtils.h"

void utils::loadModelsFrom(const std::string& path) noexcept {
    const std::filesystem::path filesystemPath{ path };
    if (std::filesystem::is_directory(filesystemPath)) {

        for (const auto& file : std::filesystem::directory_iterator(filesystemPath)) {
            loadModelsFrom(file.path().c_str());
        }

    } else if (std::filesystem::is_regular_file(filesystemPath)) {

        if (!path.ends_with(".obj"))
            return;

        try {
            Model::addModelFile(path);
        }
        catch (const std::exception& e) {
            std::cerr << "Error occurred while loading model " << path << "!" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}

bool utils::loadModelInstance(const std::string& filename, const glm::vec3 position) noexcept {
    try {
        Model::addModelFile(filename).addInstance(position);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error occurred while loading model " << filename << "!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return false;
}