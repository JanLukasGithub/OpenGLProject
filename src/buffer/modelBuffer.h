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

private:
    GLuint m_buffer_id{ 0 };
    GLsizeiptr m_size{ 0 };
};

#endif