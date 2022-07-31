#include "modelBuffer.h"

void init_vertex_attrib_array() {
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, sizeof(glm::mat4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribDivisor(4, 1);
}

Model_Buffer::Model_Buffer() {}

Model_Buffer::Model_Buffer(std::vector<glm::mat4> model_mats) : m_size(model_mats.size()) {
    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(glm::mat4), model_mats.data(), GL_DYNAMIC_DRAW);

    init_vertex_attrib_array();
}

Model_Buffer::Model_Buffer(const Model_Buffer& mbo) : m_size{ mbo.m_size }, m_free_space{ mbo.m_free_space } {
	glBindBuffer(GL_COPY_READ_BUFFER, mbo.m_buffer_id);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

    init_vertex_attrib_array();
}

Model_Buffer::Model_Buffer(Model_Buffer&& mbo) : m_size{ mbo.m_size }, m_buffer_id{ mbo.m_buffer_id }, m_free_space{ std::move(mbo.m_free_space) } {
    mbo.m_buffer_id = 0;
}

Model_Buffer::~Model_Buffer() {
    glDeleteBuffers(1, &m_buffer_id);
}

Model_Buffer& Model_Buffer::operator=(const Model_Buffer& mbo) {
    if (this == &mbo)
        return *this;

    m_size = mbo.m_size;
    m_free_space = mbo.m_free_space;

    glDeleteBuffers(1, &m_buffer_id);

	glBindBuffer(GL_COPY_READ_BUFFER, mbo.m_buffer_id);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

    init_vertex_attrib_array();

    return *this;
}

Model_Buffer& Model_Buffer::operator=(Model_Buffer&& mbo) {
    if (this == &mbo)
        return *this;

    glDeleteBuffers(1, &m_buffer_id);

    m_size = mbo.m_size;
    m_buffer_id = mbo.m_buffer_id;
    m_free_space = std::move(mbo.m_free_space);

    mbo.m_buffer_id = 0;

    return *this;
}

void Model_Buffer::set(GLsizeiptr index, glm::mat4 value) {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);

    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &value);
}

GLsizeiptr Model_Buffer::add(glm::mat4 value) {
    if (m_free_space.empty()) {
        GLuint new_buffer_id;

        glBindBuffer(GL_COPY_READ_BUFFER, m_buffer_id);

        glCreateBuffers(1, &new_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, new_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, (m_size + 1) * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size * sizeof(glm::mat4));
        glBufferSubData(GL_ARRAY_BUFFER, m_size * sizeof(glm::mat4), sizeof(glm::mat4), &value);

        glDeleteBuffers(1, &m_buffer_id);

        m_buffer_id = new_buffer_id;
        m_size++;

        return m_size - 1;
    } else {
        GLsizeiptr add_at = m_free_space.back();
        m_free_space.pop_back();

        set(add_at, value);

        return add_at;
    }
}

void Model_Buffer::remove(GLsizeiptr index) {
    set(index, glm::mat4(0));
    m_free_space.push_back(index);
}

glm::mat4 Model_Buffer::get(GLsizeiptr index) {
    glm::mat4 store_to;

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);

    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &store_to);

    return store_to;
}
