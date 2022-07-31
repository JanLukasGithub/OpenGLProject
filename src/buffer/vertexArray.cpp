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
    glVertexAttribPointer(4, sizeof(glm::mat4) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribDivisor(4, 1);
}

Vertex_Array::Vertex_Array(std::vector<Vertex> vertex_data, std::vector<glm::mat4> model_mats) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo = VertexBuffer(vertex_data.data(), vertex_data.size());
    init_vertex_attrib_array();

    m_mbo = Model_Buffer(model_mats);
    init_model_attrib_array();

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(const Vertex_Array& vao) {
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo = vao.m_vbo;
    init_vertex_attrib_array();

    m_mbo = vao.m_mbo;
    init_model_attrib_array();

    glBindVertexArray(0);
}

Vertex_Array::Vertex_Array(Vertex_Array&& vao) : m_vbo{ std::move(vao.m_vbo) }, m_mbo{ std::move(vao.m_mbo) }, m_vao_id{ vao.m_vao_id } {
    vao.m_vao_id = 0;
}

Vertex_Array::~Vertex_Array() {
    glDeleteVertexArrays(1, &m_vao_id);
}

Vertex_Array& Vertex_Array::operator=(const Vertex_Array& vao) {
    if (this == &vao)
        return *this;
    
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    m_vbo = vao.m_vbo;
    init_vertex_attrib_array();

    m_mbo = vao.m_mbo;
    init_model_attrib_array();
    
    return *this;
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

VertexBuffer& Vertex_Array::get_vertex_buffer() {
    return m_vbo;
}

Model_Buffer& Vertex_Array::get_model_buffer() {
    return m_mbo;
}