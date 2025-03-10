#pragma once

#include <string>

namespace larco {
enum class ShaderConverterStage {
  VERTEX, FRAGMENT, COMPUTE, UNDEFINED
};

enum class ShaderConverterTarget {
  GLSL, MSL, HLSL, SPIRV, UNDEFINED
};

struct ShaderConverterDesc {
  std::string shaderSource;
  ShaderConverterStage shaderType = ShaderConverterStage::UNDEFINED;
  ShaderConverterTarget shaderConverterType = ShaderConverterTarget::UNDEFINED;
};

std::string convertShader(const ShaderConverterDesc& shaderConverterDesc);
}
