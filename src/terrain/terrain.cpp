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

}

void Terrain::init() noexcept {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
	glBufferData(GL_ARRAY_BUFFER, 2 * m_sizeX * m_sizeZ, m_heightMap, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_HALF_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}