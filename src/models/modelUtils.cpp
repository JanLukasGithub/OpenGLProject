#include "modelUtils.h"

void utils::loadModelsFrom(const char* const path) noexcept {
    const std::filesystem::path filesystemPath{ path };
    if (std::filesystem::is_directory(filesystemPath)) {

        for (const auto& file : std::filesystem::directory_iterator(filesystemPath)) {
            loadModelsFrom(file.path().c_str());
        }

    } else if (std::filesystem::is_regular_file(filesystemPath)) {
        
        if (!std::string(path).ends_with(".obj"))
            return;

        try {
            Model::addModelFile((new std::string(path))->c_str());
        }
        catch (const std::exception& e) {
            std::cerr << "Error occurred while loading model " << path << "!" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}