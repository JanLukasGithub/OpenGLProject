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
    GLuint m_vao, m_vboBufferId;
    // Index Buffer
    GLuint m_iboBufferId;

    // Offset of the smallest corner from 0, 0, 0
    const int m_offsetX, m_offsetZ;

    // Width and length of the terrain chunk this represents
    const int m_sizeX, m_sizeZ;

public:
    Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const short* const heightMap) noexcept;
    Terrain(Terrain&& terrain) noexcept;
    virtual ~Terrain() noexcept;

    // Renders this part of the terrain
    virtual void render() const noexcept override;

    // Has to read from VRAM -> bad performance
    // Returns NaN when x and/or z are out of bounds
    float getHeightAt(int x, int z) const;

    // Has to write to VRAM -> bad performance
    // Does nothing when x and/or z are out of bounds
    void setHeightAt(int x, int z, float value) const;

    int getSizeX() const noexcept;
    int getSizeZ() const noexcept;
    int getOffsetX() const noexcept;
    int getOffsetZ() const noexcept;

private:
    void init(const short* const heightMap) noexcept;
};

#endif