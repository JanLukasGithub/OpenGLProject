#include "modelBuffer.h"

Model_Buffer::Model_Buffer() {
    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
}

Model_Buffer::Model_Buffer(const Model_Buffer& mbo) : m_capacity{ mbo.m_capacity }, m_free_space{ mbo.m_free_space } {
    glBindBuffer(GL_COPY_READ_BUFFER, mbo.m_buffer_id);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, m_capacity * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_capacity * sizeof(glm::mat4));
}

Model_Buffer::Model_Buffer(Model_Buffer&& mbo) : m_capacity{ mbo.m_capacity }, m_buffer_id{ mbo.m_buffer_id }, m_free_space{ std::move(mbo.m_free_space) } {
    mbo.m_buffer_id = 0;
}

Model_Buffer::~Model_Buffer() {
    m_capacity = 0;

    glDeleteBuffers(1, &m_buffer_id);
}

Model_Buffer& Model_Buffer::operator=(const Model_Buffer& mbo) {
    if (this == &mbo)
        return *this;

    m_capacity = mbo.m_capacity;
    m_free_space = mbo.m_free_space;

    glDeleteBuffers(1, &m_buffer_id);

    glBindBuffer(GL_COPY_READ_BUFFER, mbo.m_buffer_id);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, m_capacity * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_capacity * sizeof(glm::mat4));

    return *this;
}

Model_Buffer& Model_Buffer::operator=(Model_Buffer&& mbo) {
    if (this == &mbo)
        return *this;

    glDeleteBuffers(1, &m_buffer_id);

    m_capacity = mbo.m_capacity;
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
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);

        GLuint temp_buffer_id;
        glCreateBuffers(1, &temp_buffer_id);
        glBindBuffer(GL_COPY_READ_BUFFER, temp_buffer_id);
        glBufferData(GL_COPY_READ_BUFFER, m_capacity * sizeof(glm::mat4), nullptr, GL_DYNAMIC_READ);
        glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, m_capacity * sizeof(glm::mat4));
    
        glBufferData(GL_ARRAY_BUFFER, (m_capacity + 1) * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_capacity * sizeof(glm::mat4));
        glBufferSubData(GL_ARRAY_BUFFER, m_capacity * sizeof(glm::mat4), sizeof(glm::mat4), &value);

        glDeleteBuffers(1, &temp_buffer_id);

        m_capacity++;

        return m_capacity - 1;
    } else {
        GLsizeiptr add_at = m_free_space.back();
        m_free_space.pop_back();

        set(add_at, value);

        return add_at;
    }
}

void Model_Buffer::remove(GLsizeiptr index) {
    if (index == -1)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);

    glm::mat4 value{ 0.0f };
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &value);

    m_free_space.push_back(index);
}

Model_Buffer& Model_Buffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    return *this;
}

const Model_Buffer& Model_Buffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    return *this;
}

GLsizeiptr Model_Buffer::get_size() const {
    return m_capacity;
}
