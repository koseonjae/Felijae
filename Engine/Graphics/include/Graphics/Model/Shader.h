#pragma once

#include <Shader/ShaderConverter.h>

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

inline ShaderConverterStage getShaderConverterType(ShaderType shaderType) {
  switch (shaderType) {
    case ShaderType::VERTEX: return ShaderConverterStage::VERTEX;
    case ShaderType::FRAGMENT: return ShaderConverterStage::FRAGMENT;
    case ShaderType::COMPUTE: return ShaderConverterStage::COMPUTE;
    default: {}
  }
  assert(false && "Unsupported shader type");
}

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
