#pragma once

#include <string>

namespace goala {
enum class ShaderType {
  VERTEX,
  FRAGMENT,
  COMPUTE,
  UNDEFINED
};

struct ShaderDescription {
  std::string source;
  ShaderType type;
};

class Shader {
public:
  explicit Shader(ShaderType type) : m_type(type) {}

  virtual ~Shader() = default;

  ShaderType getShaderType() const {
    return m_type;
  }

protected:
  ShaderType m_type = ShaderType::UNDEFINED;
};
} // namespace goala
