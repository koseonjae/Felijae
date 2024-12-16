#include <Graphics/Metal/MetalShader.h>

#include <iostream>

using namespace goala;

namespace {
auto getNSString(std::string_view str) {
  return makeMetalRef(NS::String::string(str.data(), NS::ASCIIStringEncoding));
}
} // namespace

MetalShader::MetalShader(MetalDevice* device, ShaderDescription desc)
  : Shader(desc.type) {
  NS::Error* err = nil;

  auto source = getNSString(desc.source);
  auto library = makeMetalRef(device->getMTLDevice()->newLibrary(source.get(), nullptr, &err));
  if (err)
    std::cout << "MetalShader::MetalShader: " << err->description()->utf8String() << "\n";
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

  auto funcName = makeMetalRef(NS::String::string(shaderTypeStr.data(), NS::ASCIIStringEncoding));
  m_func = makeMetalRef(library->newFunction(funcName.get()));
}

MTL::Function* MetalShader::getFunction() { return m_func.get(); }

const MTL::Function* MetalShader::getFunction() const { return m_func.get(); }
