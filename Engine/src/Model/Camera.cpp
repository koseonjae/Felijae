#include <Engine/Model/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

void Camera::setCamera(const glm::vec3& eye, const glm::vec3& at,
                       const glm::vec3& up) {
  m_eye = eye;
  m_at = at;
  m_up = up;
}

void Camera::setProjection(float fovy, float aspectRatio, float near,
                           float far) {
  m_fovy = fovy;
  m_aspectRatio = aspectRatio;
  m_near = near;
  m_far = far;
}

glm::mat4 Camera::getViewMatrix() const {
  auto viewMat = glm::lookAt(m_eye, m_at, m_up);
  return viewMat;
}

glm::mat4 Camera::getProjMatrix() const {
  auto projMat = glm::perspective(m_fovy, m_aspectRatio, m_near, m_far);
  return projMat;
}

glm::vec3 Camera::getEye() const { return m_eye; }
