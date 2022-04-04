#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "../../lib/glm/detail/type_half.hpp"

#include "../renderer/IRenderable.h"
#include "../utils.h"
#include "../shader.h"

// Represents a part of the terrain. Multiple instances can be used
class Terrain : IRenderable {
private:
    inline static int offsetUniformLocation{0};
    inline static int sizeUniformLocation{0};
public:
    static void initUniforms(const Shader* shader);
private:
    // Vertex Buffer 
    int m_vao, m_vboBufferId;
    // Index Buffer
    int m_iboBufferId;

    // Offset of the smallest corner from 0, 0, 0
    const int m_offsetX, m_offsetZ;

    // Width and length of the terrain chunk this represents
    const int m_sizeX, m_sizeZ;

    // Terrain heights of the terrain chunk this represents. Has the size (m_sizeX / 2) * (m_sizeZ / 2) = (m_sizeX * m_sizeZ) / 4
    // (16384m / 2m)² * 2Byte ≈ 268MB. 32km ≈ 1GB
    // short just to get a 16bit value, treat as a float
    const short m_heightMap[];

public:
    // Renders this part of the terrain
    virtual void render() const noexcept override;

};

#endif