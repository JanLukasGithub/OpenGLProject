#ifndef TERRAIN_BUFFER_H_
#define TERRAIN_BUFFER_H_

#include <GL/glew.h>

#include <vector>

#include "../utils.h"

class Terrain_Buffer {
public:
    Terrain_Buffer();
    Terrain_Buffer(const float16* const height_map, const uint32 size_x, const uint32 size_z);
    Terrain_Buffer(const Terrain_Buffer& tbo);
    Terrain_Buffer(Terrain_Buffer&& tbo);
    virtual ~Terrain_Buffer();

    Terrain_Buffer& operator=(const Terrain_Buffer& tbo);
    Terrain_Buffer& operator=(Terrain_Buffer&& tbo);

    Terrain_Buffer& bind();
    const Terrain_Buffer& bind() const;
    Terrain_Buffer& unbind();
    const Terrain_Buffer& unbind() const;

private:
    void init(const float16* const height_map);
    void init_vertex_buffer(const float16* const height_map);
    void init_index_buffer();
    void copy_vertex_buffer(const GLuint old_id);
    void copy_index_buffer(const GLuint old_id);
    void destruct();

	GLuint m_ibo_id{ 0 };
	GLuint m_vbo_id{ 0 };
	GLuint m_vao_id{ 0 };

    uint32 m_size_x;
    uint32 m_size_z;
};

#endif