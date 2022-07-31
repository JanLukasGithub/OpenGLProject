#ifndef MODEL_BUFFER_H_
#define MODEL_BUFFER_H_

#include <vector>
#include <GL/glew.h>

#include "../../lib/glm/glm.hpp"

class Model_Buffer {
public:
    Model_Buffer();
    Model_Buffer(std::vector<glm::mat4> model_mats);
    Model_Buffer(const Model_Buffer& mbo);
    Model_Buffer(Model_Buffer&& mbo);
    virtual ~Model_Buffer();

    Model_Buffer& operator=(const Model_Buffer& mbo);
    Model_Buffer& operator=(Model_Buffer&& mbo);

    /** Set the model mat at the specified index. Out of bound indices will result in an OpenGL error */
    void set(GLsizeiptr index, glm::mat4 value);
    /** Add the model mat to the buffer, but might have to allocate a new buffer. Returns the index of the new value */
    GLsizeiptr add(glm::mat4 value);
    /** Remove the model mat at the index from the buffer */
    void remove(GLsizeiptr index);
    /** Get the model mat at the index */
    glm::mat4 get(GLsizeiptr index);

    Model_Buffer& bind();
    const Model_Buffer& bind() const;

private:
    GLuint m_buffer_id{ 0 };
    GLsizeiptr m_size{ 0 };

    std::vector<GLsizeiptr> m_free_space{};
};

#endif