#include "terrainManager.h"

TerrainManager::TerrainManager(const std::string& filename, const uint64 chunkSize) {
    int32 sizeX = 0, sizeZ = 0;

    stbi_uc* textureBuffer = loadImage(filename, &sizeX, &sizeZ);

    std::vector<std::vector<float16>> heightMap{};
    heightMap.resize(sizeX);

    for (int i = 0; i < heightMap.size(); i++) {
        heightMap[i].resize(sizeZ);
    }

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeZ; j++) {
            heightMap[i][j] = glm::detail::toFloat16((float)textureBuffer[i * sizeZ + j]);
        }
    }

    m_terrainChunks.resize(sizeX / chunkSize + (int)(sizeX % chunkSize != 0));

    uint64 numChunksZ = sizeZ / chunkSize + (int)(sizeZ % chunkSize != 0);
    for (int i = 0; i < m_terrainChunks.size(); i++) {
        m_terrainChunks[i].reserve(numChunksZ);
    }

    for (int x = 0; x < m_terrainChunks.size(); x++) {
        for (int z = 0; z < numChunksZ; z++) {
            uint64 chunkXOffset = x * chunkSize;
            uint64 chunkZOffset = z * chunkSize;
            uint64 thisChunkSizeX = (x != m_terrainChunks.size() - 1) ? chunkSize : sizeX % chunkSize;
            uint64 thisChunkSizeZ = (z != m_terrainChunks[x].size() - 1) ? chunkSize : sizeZ % chunkSize;

            m_terrainChunks[x].push_back(Terrain(chunkXOffset, chunkZOffset,
                getRectFrom2dArray(heightMap, chunkXOffset, chunkZOffset, thisChunkSizeX, thisChunkSizeZ)));
        }
    }

    std::cout << "Num terrain chunks: " << m_terrainChunks.size() << " " << m_terrainChunks[0].size() << std::endl;
}

void TerrainManager::render() const noexcept {
    for (int i = 0; i < m_terrainChunks.size(); i++) {
        for (int j = 0; j < m_terrainChunks[i].size(); j++) {
            m_terrainChunks[i][j].render();
        }
    }
}

stbi_uc* TerrainManager::loadImage(const std::string& filename, int* sizeX, int* sizeZ) {
    int32 bytesPerPixel = 0;

    stbi_set_flip_vertically_on_load(false);

    stbi_uc* textureBuffer = stbi_load(filename.c_str(), sizeX, sizeZ, &bytesPerPixel, 1);
    if (!textureBuffer) {
        std::cerr << "Couldn't load height map at " << filename << "! Aborting!" << std::endl;
        throw std::exception();
    }
    if (bytesPerPixel != 1) {
        std::cerr << "Height map loaded from " << filename << " uses " << bytesPerPixel << " bytes per pixel, where only 1 is supported/necessary!" << std::endl;
    }

    return textureBuffer;
}