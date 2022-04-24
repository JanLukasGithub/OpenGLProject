#include "terrain.h"

void Terrain::initUniforms(const Shader* shader) {
    Terrain::offsetUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_offset");
    Terrain::sizeUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_size");
}

Terrain::Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const float heightMap[]) noexcept : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ sizeX }, m_sizeZ{ sizeZ } {
    float16 halfFloatHeightMap[sizeX * sizeZ]{};

    for (int x = 0; x < sizeX; x++) {
        for (int z = 0; z < sizeZ; z++) {
            halfFloatHeightMap[z * sizeX + x] = glm::detail::toFloat16(heightMap[z * sizeX + x]);
        }
    }

    init(halfFloatHeightMap);
}

Terrain::Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const float16 heightMap[]) noexcept : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ sizeX }, m_sizeZ{ sizeZ } {
    init(heightMap);
}

Terrain::Terrain(Terrain&& terrain) noexcept : m_offsetX{ terrain.m_offsetX }, m_offsetZ{ terrain.m_offsetZ }, m_sizeX{ terrain.m_sizeX },
m_sizeZ{ terrain.m_sizeZ }, m_iboBufferId{ terrain.m_iboBufferId }, m_vboBufferId{ terrain.m_vboBufferId }, m_vao{ terrain.m_vao } {
    terrain.m_iboBufferId = 0;
    terrain.m_vboBufferId = 0;
    terrain.m_vao = 0;
}

Terrain::~Terrain() noexcept {
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
        glDrawElements(GL_TRIANGLE_STRIP, m_sizeX * 2, GL_UNSIGNED_INT, (void*)(i * m_sizeX * 2 * sizeof(uint32)));
    }
}

void Terrain::init(const float16* const heightMap) noexcept {
    // Vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Vbo
    glGenBuffers(1, &m_vboBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*heightMap) * m_sizeX * m_sizeZ, heightMap, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_HALF_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    // Ibo
    glGenBuffers(1, &m_iboBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboBufferId);
    // Null-initialize ibo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sizeX * (m_sizeZ - 1) * sizeof(uint32) * 2, nullptr, GL_STATIC_DRAW);

    // A buffer with m_sizeX * 2 indices per strip, tightly packed together m_sizeZ - 1 times
    for (uint32 z = 0; z < m_sizeZ - 1; z++) {
        for (uint32 x = z * m_sizeX; x < (z + 1) * m_sizeX; x++) {
            uint32 values[] = { x, x + m_sizeX };
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, x * sizeof(values), sizeof(values), &values);
        }
    }
}

float Terrain::getHeightAt(int x, int z) const {
    if (x >= m_sizeX || z >= m_sizeZ || x < 0 || z < 0)
        return NAN;

    float16 storeTo{};

    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
    glGetBufferSubData(GL_ARRAY_BUFFER, (z * m_sizeX + x) * sizeof(storeTo), sizeof(storeTo), &storeTo);

    return glm::detail::toFloat32(storeTo);
}

void Terrain::setHeightAt(int x, int z, float value) const {
    if (x >= m_sizeX || z >= m_sizeZ || x < 0 || z < 0)
        return;

    float16 halfFloatValue{ glm::detail::toFloat16(value) };

    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
    // Ensures that the data can be written immediately instead of waiting for rendering to finish
    glInvalidateBufferData(m_vboBufferId);
    glBufferSubData(GL_ARRAY_BUFFER, (z * m_sizeX + x) * sizeof(halfFloatValue), sizeof(halfFloatValue), &halfFloatValue);
}

int Terrain::getOffsetX() const noexcept { return m_offsetX; }
int Terrain::getOffsetZ() const noexcept { return m_offsetZ; }
int Terrain::getSizeX() const noexcept { return m_sizeX; }
int Terrain::getSizeZ() const noexcept { return m_sizeZ; }