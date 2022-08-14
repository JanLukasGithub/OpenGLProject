#include "modelInstance.h"

Model_Instance::Model_Instance(const std::string& filename) : m_model_index{ Model_Manager::get_model_index(filename) }, m_model_mat{ 1.0f },
m_instance_index{ Model_Manager::get_from_index(m_model_index).add_instance() } {}

Model_Instance::Model_Instance(uint32 model_index) : m_model_mat{ 1.0f }, m_model_index{ model_index },
m_instance_index{ Model_Manager::get_from_index(m_model_index).add_instance(m_model_mat) } {}

Model_Instance::Model_Instance(uint32 model_index, glm::mat4 model_mat) : m_model_mat{ model_mat }, m_model_index{ model_index },
m_instance_index{ Model_Manager::get_from_index(m_model_index).add_instance(m_model_mat) } {}

Model_Instance::Model_Instance(uint32 model_index, glm::vec3 position) : m_model_index{ model_index }, m_model_mat{ glm::translate(glm::mat4(1.0f), position) },
m_instance_index{ Model_Manager::get_from_index(m_model_index).add_instance(m_model_mat) } {}

Model_Instance::Model_Instance(const Model_Instance& model) : m_model_index{ model.m_model_index }, m_model_mat{ model.m_model_mat },
m_instance_index{ Model_Manager::get_from_index(m_model_index).add_instance(m_model_mat) } {}

Model_Instance::Model_Instance(Model_Instance&& model) : m_model_index{ model.m_model_index }, m_model_mat{ model.m_model_mat },
m_instance_index{ model.m_instance_index } {
    model.m_instance_index = -1;
}

Model_Instance::~Model_Instance() {
    Model_Manager::get_from_index(m_model_index).remove_instance(m_instance_index);
}

Model_Instance& Model_Instance::operator=(const Model_Instance& model) {
    if (this == &model)
        return *this;

    Model_Manager::get_from_index(m_model_index).remove_instance(m_instance_index);

    m_model_index = model.m_model_index;
    m_model_mat = model.m_model_mat;

    m_instance_index = Model_Manager::get_from_index(m_model_index).add_instance(m_model_mat);

    return *this;
}

Model_Instance& Model_Instance::operator=(Model_Instance&& model) {
    if (this == &model)
        return *this;

    Model_Manager::get_from_index(m_model_index).remove_instance(m_instance_index);

    m_model_index = model.m_model_index;
    m_model_mat = model.m_model_mat;
    m_instance_index = model.m_instance_index;

    model.m_instance_index = -1;

    return *this;
}

void Model_Instance::set_model_mat(glm::mat4 model_mat) {
    m_model_mat = model_mat;
    update();
}

void Model_Instance::apply_model_mat(glm::mat4 model_mat) {
    m_model_mat = model_mat * m_model_mat;
    update();
}

void Model_Instance::translate(glm::vec3 translation) {
    m_model_mat = glm::translate(m_model_mat, translation);
    update();
}

void Model_Instance::rotate(float degrees, glm::vec3 axis) {
    m_model_mat = glm::rotate(m_model_mat, degrees, axis);
    update();
}

void Model_Instance::scale(glm::vec3 scale) {
    m_model_mat = glm::scale(m_model_mat, scale);
    update();
}

void Model_Instance::update() {
    Model_Manager::get_from_index(m_model_index).set_instance(m_instance_index, m_model_mat);
}
