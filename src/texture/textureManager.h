#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <vector>
#include <string>

#include "texture.h"
#include "../utils.h"

namespace Texture_Manager {
    /** Returns the texture from the specified file */
    uint32 get_texture_index(std::string filename);
    /** Returns the texture from the index. */
    const Texture& get_from_index(int64 index);
}

#endif