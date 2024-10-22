#pragma once

#include <glm/glm.hpp>

class Camera final {
 public:
  void setCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up);
  void setProjection(float near, float far, float fovy, float aspectRatio);

 private:
  float m_near{0.0f};
  float m_far{0.0f};
  float m_fovy{0.0f};
  float m_aspectRatio{0.0f}; // width / height
  glm::vec3 m_eye;
  glm::vec3 m_at;
  glm::vec3 m_up;
};