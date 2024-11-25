#pragma once

#include <glm/glm.hpp>
#include <string_view>

namespace goala {

class Texture;
class Uniforms;

class Program {
 public:
  Program() = default;
  virtual ~Program() = default;

  virtual void initialize(std::string_view vsPath, std::string_view fsPath) = 0;

  virtual void bind(Uniforms* uniforms) = 0;
};

} // namespace goala
