#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include <vector>

#include "../../lib/glm/glm.hpp"

#include "../utils.h"
#include "vertexBuffer.h"
#include "modelBuffer.h"

class Vertex_Array {
public:
    Vertex_Array(std::vector<Vertex> vertex_data, const Model_Buffer& mbo);
    Vertex_Array(const Vertex_Array& vao);
    Vertex_Array(Vertex_Array&& vao);
    virtual ~Vertex_Array();

    Vertex_Array& bind();
    Vertex_Array& unbind();

private:
    GLuint m_vao_id;

    VertexBuffer m_vbo;
    const Model_Buffer& m_mbo;
};

#endif