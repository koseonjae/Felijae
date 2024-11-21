#pragma once

#include <Graphics/Metal/MetalDevice.h>

#include <Metal/MTLLibrary.hpp>

#include <string_view>

enum class ShaderType {
  VERTEX,
  FRAGMENT,
  COMPUTE
};

class MetalShader {
 public:
  MetalShader(MetalDevice* device, std::string_view filePath, ShaderType type);
  MTL::Function* get() const;

 private:
  MetalRef<MTL::Function> m_func;
};