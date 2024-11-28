#pragma once

#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Model/Shader.h>

#include <Metal/MTLLibrary.hpp>

namespace goala {
class MetalShader : public Shader {
public:
  MetalShader(MetalDevice* device, ShaderDescription desc);
  MTL::Function* getFunction();
  const MTL::Function* getFunction() const;

private:
  MetalRef<MTL::Function> m_func;
};
} // namespace goala
