#include "textureManager.h"

static std::vector<Texture> textures{};

uint32 Texture_Manager::get_texture_index(std::string filename) {
    if (textures.empty())
        textures.push_back(Texture{});
    
    if (filename.compare("") == 0)
        return 0;

    auto it = std::find(textures.begin(), textures.end(), filename);

    if (it != textures.end())
        return it - textures.begin();
    else {
        textures.push_back(Texture(filename));

        return textures.size() - 1;
    }
}

const Texture& Texture_Manager::get_from_index(int64 index) {
    return textures[index];
}