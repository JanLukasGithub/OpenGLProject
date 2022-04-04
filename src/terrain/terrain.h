#ifndef TERRAIN_H_
#define TERRAIN_H_

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
    // Offset of the smallest corner from 0, 0, 0
    const int m_offsetX, m_offsetZ;

    // Width and length of the terrain chunk this represents
    const int m_sizeX, m_sizeZ;

    // Terrain heights of the terrain chunk this represents. Has the size m_sizeX * m_sizeZ
    const float32 m_heightMap[];

public:
    // Renders this part of the terrain
    virtual void render() const noexcept override;

};

#endif