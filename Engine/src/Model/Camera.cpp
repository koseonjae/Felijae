#include <Model/Camera.h>

void Camera::setCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up) {
  m_eye = eye;
  m_at = at;
  m_up = up;
}

void Camera::setProjection(float near, float far, float fovy, float aspectRatio) {
  m_near = near;
  m_far = far;
  m_fovy = fovy;
  m_aspectRatio = aspectRatio;
}