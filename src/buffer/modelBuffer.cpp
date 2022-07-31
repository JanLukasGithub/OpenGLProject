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
    glBufferData(GL_ARRAY_BUFFER, m_size, model_mats.data(), GL_DYNAMIC_DRAW);

    init_vertex_attrib_array();
}

Model_Buffer::Model_Buffer(const Model_Buffer& mbo) : m_size{ mbo.m_size } {
	glBindBuffer(GL_COPY_READ_BUFFER, mbo.m_buffer_id);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size);

    init_vertex_attrib_array();
}

Model_Buffer::Model_Buffer(Model_Buffer&& mbo) : m_size{ mbo.m_size }, m_buffer_id{ mbo.m_buffer_id } {
    mbo.m_buffer_id = 0;
}

Model_Buffer::~Model_Buffer() {
    glDeleteBuffers(1, &m_buffer_id);
}

Model_Buffer& Model_Buffer::operator=(const Model_Buffer& mbo) {
    if (this == &mbo)
        return *this;

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

    mbo.m_buffer_id = 0;

    return *this;
}
