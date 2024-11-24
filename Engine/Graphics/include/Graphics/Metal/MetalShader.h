#pragma once

#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Model/Shader.h>

#include <Metal/MTLLibrary.hpp>
#include <string_view>

namespace goala {

class MetalShader : public Shader {
 public:
  MetalShader(MetalDevice* device, std::string_view filePath, ShaderType type);
  MTL::Function* getFunction() const;

 private:
  MetalRef<MTL::Function> m_func;
};

} // namespace goala
