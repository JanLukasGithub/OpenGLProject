#include "modelUtils.h"

void utils::loadModelsFrom(const char* const path) noexcept {
    const std::filesystem::path filesystemPath{ path };
    std::error_code ec;
    if (std::filesystem::is_directory(filesystemPath, ec)) {

        for (const auto& file : std::filesystem::directory_iterator(filesystemPath)) {
            loadModelsFrom(file.path().c_str());
        }

    } else if (std::filesystem::is_regular_file(filesystemPath, ec)) {
        // Only load obj files for now, ignore textures etc.
        if (!std::string(path).ends_with(".obj"))
            return;

        try {
            Model::addModelFile((new std::string(path))->c_str());
        } catch (const std::exception& e) {
		std::cerr << "Error occurred while loading model " << path << "!" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    if (ec) {
        std::cerr << ec.message() << std::endl;
    }
}