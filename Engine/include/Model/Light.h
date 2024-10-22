#pragma once

#include <glm/glm.hpp>

class Light final {
 public:
  inline void setLightColor(const glm::vec3 &color) {
    m_lightColor = color;
  }
  inline void setLightDirection(const glm::vec3 &dir) {
    m_lightDir = dir;
  }
  inline glm::vec3 getLightColor() const {
    return m_lightColor;
  };
  inline glm::vec3 getLightDirection() const {
    return m_lightDir;
  };

 private:
  glm::vec3 m_lightColor;
  glm::vec3 m_lightDir;
};