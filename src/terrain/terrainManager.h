#ifndef TERRAIN_MANAGER_H_
#define TERRAIN_MANAGER_H_

#include <array>

#include "terrain.h"

class TerrainManager : IRenderable {
public:
    // chunkSize is how long one chunk of terrain should be (they're quadratic)
    TerrainManager(const std::string& filename, const uint64 chunkSize);

    void render() const noexcept override;
protected:
    std::vector<std::vector<Terrain>> m_terrainChunks;

    static stbi_uc* loadImage(const std::string& filename, int* sizeX, int* sizeZ);
};

#endif