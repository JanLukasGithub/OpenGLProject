#include "terrain.h"

void Terrain::initUniforms(const Shader* shader) {
    Terrain::offsetUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_offset");
    Terrain::sizeUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_size");
}

Terrain::Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const short* const heightMap) noexcept : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ sizeX }, m_sizeZ{ sizeZ }, m_heightMap{ new short[sizeX * sizeZ] } {
    memcpy(m_heightMap, heightMap, 2 * sizeX * sizeZ);

    init();
}

Terrain::Terrain(Terrain&& terrain) noexcept : m_offsetX{ terrain.m_offsetX }, m_offsetZ{ terrain.m_offsetZ }, m_sizeX{ terrain.m_sizeX },
m_sizeZ{ terrain.m_sizeZ }, m_heightMap{ terrain.m_heightMap }, m_iboBufferId{ terrain.m_iboBufferId }, m_vboBufferId{ terrain.m_vboBufferId },
m_vao{ terrain.m_vao } {
    terrain.m_heightMap = nullptr;

    terrain.m_iboBufferId = 0;
    terrain.m_vboBufferId = 0;
    terrain.m_vao = 0;
}

Terrain::~Terrain() noexcept {
    delete[] m_heightMap;

    glDeleteBuffers(1, &m_iboBufferId);
    glDeleteBuffers(1, &m_vboBufferId);
    glDeleteVertexArrays(1, &m_vao);
}

void Terrain::render() const noexcept {
    glUniform1i(sizeUniformLocation, m_sizeX);
    glUniform2i(offsetUniformLocation, m_offsetX, m_offsetZ);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboBufferId);

    for (int i = 0; i < m_sizeZ - 1; i++) {
        glDrawRangeElements(GL_TRIANGLE_STRIP, i * m_sizeX * 2, (i + 1) * m_sizeX * 2, m_sizeX * 2, GL_UNSIGNED_INT, 0);
    }
}

void Terrain::init() noexcept {
    // Vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Vbo
    glGenBuffers(1, &m_vboBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, 2 * m_sizeX * m_sizeZ, m_heightMap, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_HALF_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    // Ibo
    glGenBuffers(1, &m_iboBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboBufferId);
    // Actually allocates memory for the buffer with the size m_sizeX * m_sizeZ * 4 but doesn't initialize that memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeX * m_sizeZ * 4, nullptr, GL_STATIC_DRAW);

    // A buffer with m_sizeX * 2 per strip, tightly packed together m_sizeZ - 1 times
    for (uint32 z = 0; z < m_sizeZ - 1; z++) {
        for (uint32 x = z * m_sizeX; x < (z + 1) * m_sizeX; x++) {
            uint32 values[] = { x, x + m_sizeX };
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, x * sizeof(values), sizeof(values), &values);
        }
    }
}