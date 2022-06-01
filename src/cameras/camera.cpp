#include "camera.h"

Camera::Camera(float fov, float width, float height) : m_projection{ glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f) }, m_viewProj{ m_projection * m_view } {}

Camera::Camera(float fov, float width, float height, float near, float far) : m_projection{ glm::perspective(fov / 2.0f, width / height, near, far) },
m_viewProj{ m_projection * m_view } {}

Camera::~Camera() {}

void Camera::update() {
	m_viewProj = m_projection * m_view;
}

void Camera::translate(glm::vec3 vec3) {
	m_position += vec3;
	m_view = glm::translate(m_view, vec3 * -1.0f);
}

void Camera::handleInputs(SdlEventHandler* handler, float32 delta) {}

const glm::mat4& Camera::getProjection() const noexcept {
	return m_projection;
}

const glm::mat4& Camera::getViewProjection() const noexcept {
	return m_viewProj;
}

const glm::mat4& Camera::getView() const noexcept {
	return m_view;
}

const glm::vec3& Camera::getPosition() const noexcept {
	return m_position;
}