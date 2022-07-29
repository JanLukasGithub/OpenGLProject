#include "terrain.h"

void Terrain::initUniforms(const Shader* shader) {
    Terrain::offsetUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_offset");
    Terrain::sizeUniformLocation = glGetUniformLocation(shader->getShaderId(), "u_size");
}

Terrain::Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const float heightMap[]) : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ sizeX }, m_sizeZ{ sizeZ } {
    float16 halfFloatHeightMap[sizeX * sizeZ]{};

    for (int i = 0; i < sizeX * sizeZ; i++) {
        halfFloatHeightMap[i] = glm::detail::toFloat16(heightMap[i]);
    }

    m_buffer = Terrain_Buffer(halfFloatHeightMap, m_sizeX, m_sizeZ);
}

Terrain::Terrain(const int offsetX, const int offsetZ, const int sizeX, const int sizeZ, const float16 heightMap[]) : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ sizeX }, m_sizeZ{ sizeZ } {
    m_buffer = Terrain_Buffer(heightMap, m_sizeX, m_sizeZ);
}

Terrain::Terrain(const int offsetX, const int offsetZ, const boost::multi_array<float16, 2>& heightMap) : m_offsetX{ offsetX },
m_offsetZ{ offsetZ }, m_sizeX{ heightMap.size() }, m_sizeZ{ heightMap[0].size() } {

    m_buffer = Terrain_Buffer(heightMap.data(), m_sizeX, m_sizeZ);
}

Terrain::Terrain(const int offsetX, const int offsetZ, const std::string& filename) : m_offsetX{ offsetX },
m_offsetZ{ offsetZ } {
    int32 bytesPerPixel = 0;

    stbi_set_flip_vertically_on_load(false);

    stbi_uc* textureBuffer = stbi_load(filename.c_str(), &m_sizeX, &m_sizeZ, &bytesPerPixel, 1);
    if (!textureBuffer) {
        std::cerr << "Couldn't load height map at " << filename << "! Aborting!" << std::endl;
        throw std::exception();
    }
    if (bytesPerPixel != 1) {
        std::cerr << "Height map loaded from " << filename << " uses " << bytesPerPixel << " bytes per pixel, where only 1 is supported/necessary!" << std::endl;
    }

    std::vector<float16> heightMap{};
    heightMap.resize(m_sizeX * m_sizeZ);

    for (int i = 0; i < m_sizeX * m_sizeZ; i++) {
        heightMap[i] = glm::detail::toFloat16((float)textureBuffer[i]);
    }

    m_buffer = Terrain_Buffer(heightMap.data(), m_sizeX, m_sizeZ);
}

Terrain::Terrain(const Terrain& terrain) : m_offsetX{ terrain.m_offsetX }, m_offsetZ{ terrain.m_offsetZ }, m_sizeX{ terrain.m_sizeX },
m_sizeZ{ terrain.m_sizeZ }, m_buffer{ terrain.m_buffer } {
}

Terrain::Terrain(Terrain&& terrain) : m_offsetX{ terrain.m_offsetX }, m_offsetZ{ terrain.m_offsetZ }, m_sizeX{ terrain.m_sizeX },
m_sizeZ{ terrain.m_sizeZ }, m_buffer{ terrain.m_buffer } {
}

Terrain& Terrain::operator=(const Terrain& terrain) {
    if (this == &terrain)
        return *this;

    this->m_offsetX = terrain.m_offsetX;
    this->m_offsetZ = terrain.m_offsetZ;
    this->m_sizeX = terrain.m_sizeX;
    this->m_sizeZ = terrain.m_sizeZ;
    this->m_buffer = terrain.m_buffer;

    return *this;
}

Terrain& Terrain::operator=(Terrain&& terrain) {
    if (this == &terrain)
        return *this;

    this->m_offsetX = terrain.m_offsetX;
    this->m_offsetZ = terrain.m_offsetZ;
    this->m_sizeX = terrain.m_sizeX;
    this->m_sizeZ = terrain.m_sizeZ;
    this->m_buffer = terrain.m_buffer;

    return *this;
}

Terrain::~Terrain() noexcept {}

void Terrain::render() const noexcept {
    glUniform1i(sizeUniformLocation, m_sizeX);
    glUniform2i(offsetUniformLocation, m_offsetX, m_offsetZ);

    m_buffer.bind();

    glDrawElements(GL_TRIANGLE_STRIP, m_buffer.get_num_indices(), GL_UNSIGNED_INT, 0);
}

// float Terrain::getHeightAt(int x, int z) const {
//     if (x >= m_sizeX || z >= m_sizeZ || x < 0 || z < 0)
//         return NAN;

//     float16 storeTo{};

//     glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
//     glGetBufferSubData(GL_ARRAY_BUFFER, (z * m_sizeX + x) * sizeof(storeTo), sizeof(storeTo), &storeTo);

//     return glm::detail::toFloat32(storeTo);
// }

// void Terrain::setHeightAt(int x, int z, float value) const {
//     if (x >= m_sizeX || z >= m_sizeZ || x < 0 || z < 0)
//         return;

//     float16 halfFloatValue{ glm::detail::toFloat16(value) };

//     glBindBuffer(GL_ARRAY_BUFFER, m_vboBufferId);
//     // Ensures that the data can be written immediately instead of waiting for rendering to finish
//     glInvalidateBufferData(m_vboBufferId);
//     glBufferSubData(GL_ARRAY_BUFFER, (z * m_sizeX + x) * sizeof(halfFloatValue), sizeof(halfFloatValue), &halfFloatValue);
// }

int Terrain::getOffsetX() const noexcept { return m_offsetX; }
int Terrain::getOffsetZ() const noexcept { return m_offsetZ; }
int Terrain::getSizeX() const noexcept { return m_sizeX; }
int Terrain::getSizeZ() const noexcept { return m_sizeZ; }