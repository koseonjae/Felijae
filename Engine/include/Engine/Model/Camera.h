#pragma once

#include <glm/glm.hpp>

namespace goala {

class Camera final {
 public:
  void setCamera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
  void setProjection(float fovy, float aspectRatio, float near, float far);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjMatrix() const;
  glm::vec3 getEye() const;

 private:
  float m_near = 0.0f;
  float m_far = 0.0f;
  float m_fovy = 0.0f;
  float m_aspectRatio = 0.0f; // width / height
  glm::vec3 m_eye{};
  glm::vec3 m_at{};
  glm::vec3 m_up{};
};

} // namespace goala
