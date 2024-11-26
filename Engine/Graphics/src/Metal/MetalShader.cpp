#include <Graphics/Metal/MetalShader.h>

using namespace goala;

namespace {
inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

MetalShader::MetalShader(MetalDevice* device, std::string_view source, ShaderType type)
  : Shader(type) {
  NS::Error* err = nil;

  auto library =
    MetalRef(device->getMTLDevice()->newLibrary(getNSString(source), nullptr, &err));
  assert(library && "Failed to create library");

  std::string shaderTypeStr;
  if (type == ShaderType::VERTEX)
    shaderTypeStr = "vertexShader";
  else if (type == ShaderType::FRAGMENT)
    shaderTypeStr = "fragmentShader";
  else if (type == ShaderType::COMPUTE)
    shaderTypeStr = "computeShader";
  else
    assert(false && "undefined shader type");

  auto funcName =
    NS::String::string(shaderTypeStr.data(), NS::ASCIIStringEncoding);
  m_func = MetalRef(library->newFunction(funcName));
}

MTL::Function* MetalShader::getFunction() { return m_func.get(); }

const MTL::Function* MetalShader::getFunction() const { return m_func.get(); }