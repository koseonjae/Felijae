#pragma once

namespace goala {
enum class ShaderType {
  VERTEX,
  FRAGMENT,
  COMPUTE,
  UNDEFINED
};

class Shader {
public:
  Shader(ShaderType type) : m_type(type) {}

  virtual ~Shader() = default;

  ShaderType getShaderType() const {
    return m_type;
  }

protected:
  ShaderType m_type = ShaderType::UNDEFINED;
};
} // namespace goala
