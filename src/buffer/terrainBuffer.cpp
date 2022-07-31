#include "terrainBuffer.h"

void Terrain_Buffer::init(const float16* const height_map) {
    init_vertex_buffer(height_map);

    init_index_buffer();
}

void Terrain_Buffer::init_vertex_buffer(const float16* const height_map) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    glGenBuffers(1, &m_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*height_map) * m_size_x * m_size_z, height_map, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_HALF_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void Terrain_Buffer::init_index_buffer() {
    size_t num_rows = m_size_z - 1;
    size_t num_elements_per_row = (2 * m_size_x) - 1;
    m_num_indices = (num_elements_per_row * num_rows) + 1;

    std::vector<uint32> ibo_elements{};
    ibo_elements.reserve(m_num_indices);
    ibo_elements.push_back(0);

    // for each row
    for (int row = 0; row < num_rows; row++) {
        int32 subtract = (row % 2 == 0) ? -(m_size_x - 1) : -(m_size_x + 1);

        // for each value
        for (int element_in_row = 0; element_in_row < num_elements_per_row; element_in_row++) {
            int32 diff = (element_in_row % 2 == 0) ? m_size_x : subtract;

            ibo_elements.push_back(ibo_elements.back() + diff);
        }
    }

    glGenBuffers(1, &m_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_num_indices * sizeof(ibo_elements[0]), ibo_elements.data(), GL_STATIC_DRAW);
}

void Terrain_Buffer::copy_vertex_buffer(const GLuint old_vbo_id) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    glBindBuffer(GL_COPY_READ_BUFFER, old_vbo_id);

    glGenBuffers(1, &m_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float16) * m_size_x * m_size_z, nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, sizeof(float16) * m_size_x * m_size_z);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_HALF_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void Terrain_Buffer::copy_index_buffer(const GLuint old_ibo_id) {
    size_t num_rows = m_size_z - 1;
    size_t num_elements_per_row = (2 * m_size_x) - 1;
    size_t num_ibo_elements = (num_elements_per_row * num_rows) + 1;

    glBindBuffer(GL_COPY_READ_BUFFER, old_ibo_id);

    glGenBuffers(1, &m_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_ibo_elements * sizeof(uint32), nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, num_ibo_elements * sizeof(uint32));
}

void Terrain_Buffer::destruct() {
    glDeleteBuffers(1, &m_vbo_id);
    glDeleteVertexArrays(1, &m_vao_id);

    glDeleteBuffers(1, &m_ibo_id);
}

Terrain_Buffer::Terrain_Buffer() : m_size_x{ 0 }, m_size_z{ 0 }, m_ibo_id{ 0 }, m_vao_id{ 0 }, m_vbo_id { 0 }, m_num_indices{ 0 } {}

Terrain_Buffer::Terrain_Buffer(const float16* const height_map, const uint32 size_x, const uint32 size_z) : m_size_x{ size_x }, m_size_z{ size_z } {
    init(height_map);
}

Terrain_Buffer::Terrain_Buffer(const Terrain_Buffer& tbo) : m_size_x{ tbo.m_size_x }, m_size_z{ tbo.m_size_z }, m_num_indices{ tbo.m_num_indices } {
    copy_vertex_buffer(tbo.m_vbo_id);
    copy_index_buffer(tbo.m_ibo_id);
}

Terrain_Buffer::Terrain_Buffer(Terrain_Buffer&& tbo) : m_size_x{ tbo.m_size_x }, m_size_z{ tbo.m_size_z }, m_ibo_id{ tbo.m_ibo_id }, m_vbo_id{ tbo.m_vbo_id },
    m_vao_id{ tbo.m_vao_id }, m_num_indices{ tbo.m_num_indices } {

    tbo.m_ibo_id = 0;
    tbo.m_vao_id = 0;
    tbo.m_vbo_id = 0;
}

Terrain_Buffer::~Terrain_Buffer() {
    destruct();
}

Terrain_Buffer& Terrain_Buffer::operator=(const Terrain_Buffer& tbo) {
    if (&tbo == this)
        return *this;

    destruct();

    m_size_x = tbo.m_size_x;
    m_size_z = tbo.m_size_z;
    m_num_indices = tbo.m_num_indices;

    copy_vertex_buffer(tbo.m_vbo_id);
    copy_index_buffer(tbo.m_ibo_id);

    return *this;
}

Terrain_Buffer& Terrain_Buffer::operator=(Terrain_Buffer&& tbo) {
    if (&tbo == this)
        return *this;

    destruct();

    m_size_x = tbo.m_size_x;
    m_size_z = tbo.m_size_z;
    m_ibo_id = tbo.m_ibo_id;
    m_vao_id = tbo.m_vao_id;
    m_vbo_id = tbo.m_vbo_id;
    m_num_indices = tbo.m_num_indices;

    tbo.m_ibo_id = 0;
    tbo.m_vao_id = 0;
    tbo.m_vbo_id = 0;

    return *this;
}

Terrain_Buffer& Terrain_Buffer::bind() {
	glBindVertexArray(m_vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
	return *this;
}

const Terrain_Buffer& Terrain_Buffer::bind() const {
	glBindVertexArray(m_vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
	return *this;
}

Terrain_Buffer& Terrain_Buffer::unbind() {
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return *this;
}

const Terrain_Buffer& Terrain_Buffer::unbind() const {
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return *this;
}

uint32 Terrain_Buffer::get_num_indices() const noexcept {
    return m_num_indices;
}
