#include "vertexArray.h"

void init_vertex_attrib_array() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(Vertex::normal) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, sizeof(Vertex::tangent) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, sizeof(Vertex::textureCoords) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
}

void init_model_attrib_array() {
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, sizeof(glm::vec4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 0));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, sizeof(glm::vec4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 1));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, sizeof(glm::vec4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));
    glVertexAttribDivisor(6, 1);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, sizeof(glm::vec4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));
    glVertexAttribDivisor(7, 1);
}

Vertex_Array::Vertex_Array(std::vector<Vertex> vertex_data, const Model_Buffer& mbo) : m_vbo{ vertex_data.data(), vertex_data.size() }, m_mbo{ mbo } {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo.bind();
    init_vertex_attrib_array();

    m_mbo.bind();
    init_model_attrib_array();

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(const Vertex_Array& vao) : m_vbo{ vao.m_vbo }, m_mbo{ vao.m_mbo } {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo.bind();
    init_vertex_attrib_array();

    m_mbo.bind();
    init_model_attrib_array();

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(Vertex_Array&& vao) : m_vbo{ std::move(vao.m_vbo) }, m_mbo{ vao.m_mbo }, m_vao_id{ vao.m_vao_id } {
    vao.m_vao_id = 0;
}

Vertex_Array::~Vertex_Array() {
    glDeleteVertexArrays(1, &m_vao_id);
}

Vertex_Array& Vertex_Array::bind() {
    glBindVertexArray(m_vao_id);
    return *this;
}

Vertex_Array& Vertex_Array::unbind() {
    glBindVertexArray(0);
    return *this;
}
