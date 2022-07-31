#include "vertexArray.h"

Vertex_Array::Vertex_Array(std::vector<Vertex> vertex_data, std::vector<glm::mat4> model_mats) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo = VertexBuffer(vertex_data.data(), vertex_data.size());
    m_mbo = Model_Buffer(model_mats);

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(const Vertex_Array& vao) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo = vao.m_vbo;
    m_mbo = vao.m_mbo;

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(Vertex_Array&& vao) : m_vbo{ std::move(vao.m_vbo) }, m_mbo{ std::move(vao.m_mbo) }, m_vao_id{ vao.m_vao_id } {
    vao.m_vao_id = 0;
}

Vertex_Array::~Vertex_Array() {
    glDeleteVertexArrays(1, &m_vao_id);
}

Vertex_Array& Vertex_Array::operator=(Vertex_Array&& vao) {
    if (this == &vao)
        return *this;

    m_mbo = std::move(vao.m_mbo);
    m_vbo = std::move(vao.m_vbo);
    m_vao_id = vao.m_vao_id;

    vao.m_vao_id = 0;

    return *this;
}

Vertex_Array& Vertex_Array::bind() {
    glBindVertexArray(m_vao_id);
    return *this;
}

Vertex_Array& Vertex_Array::unbind() {
    glBindVertexArray(0);
    return *this;
}