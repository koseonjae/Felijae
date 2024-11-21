#pragma once

#include <glm/glm.hpp>
#include <string_view>

namespace goala {

class Texture;

class Program {
 public:
  Program() = default;
  virtual ~Program() = default;

  virtual void initialize(std::string_view vsPath, std::string_view fsPath) = 0;

  virtual void bind() = 0;

  virtual void setUniform(std::string_view name, const glm::vec3& vec3) = 0;

  virtual void setUniform(std::string_view name, const glm::mat4& mat4) = 0;

  virtual void setTexture(std::string_view name,
                          std::shared_ptr<Texture> texture) = 0;
};

} // namespace goala
