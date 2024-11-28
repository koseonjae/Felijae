#include <Graphics/Metal/MetalShader.h>

using namespace goala;

namespace {
inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

MetalShader::MetalShader(MetalDevice* device, ShaderDescription desc)
  : Shader(desc.type) {
  NS::Error* err = nil;

  auto library = makeMetalRef(device->getMTLDevice()->newLibrary(getNSString(desc.source), nullptr, &err));
  assert(library && "Failed to create library");

  std::string shaderTypeStr;
  if (desc.type == ShaderType::VERTEX)
    shaderTypeStr = "vertexShader";
  else if (desc.type == ShaderType::FRAGMENT)
    shaderTypeStr = "fragmentShader";
  else if (desc.type == ShaderType::COMPUTE)
    shaderTypeStr = "computeShader";
  else
    assert(false && "undefined shader type");

  auto funcName = NS::String::string(shaderTypeStr.data(), NS::ASCIIStringEncoding);
  m_func = makeMetalRef(library->newFunction(funcName));
}

MTL::Function* MetalShader::getFunction() { return m_func.get(); }

const MTL::Function* MetalShader::getFunction() const { return m_func.get(); }
