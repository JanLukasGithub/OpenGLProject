#ifndef MODEL_INSTANCE_H_
#define MODEL_INSTANCE_H_

#include "../../lib/glm/glm.hpp"

#include "modelManager.h"

class Model_Instance {
public:
    Model_Instance(const std::string& filename);
    Model_Instance(uint32 model_index);
    Model_Instance(uint32 model_index, glm::mat4 model_mat);
    Model_Instance(uint32 model_index, glm::vec3 position);
    Model_Instance(const Model_Instance& model);
    Model_Instance(Model_Instance&& model);

    virtual ~Model_Instance();

    Model_Instance& operator=(const Model_Instance& model);
    Model_Instance& operator=(Model_Instance&& model);

    void set_model_mat(glm::mat4 model_mat);

    void apply_model_mat(glm::mat4 model_mat);
    void translate(glm::vec3 translation);
    void scale(glm::vec3 scale);
    void rotate(float degrees, glm::vec3 axis);

private:
    /** Updates the model mat stored on the GPU */
    void update();

    uint32 m_model_index;
    glm::mat4 m_model_mat;
    GLsizeiptr m_instance_index;
};

#endif