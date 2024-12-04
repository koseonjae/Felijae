#include <Base/File/File.h>
#include <Shader/ShaderConverter.h>

#include <ostream>
#include <iostream>

using namespace goala;

int main() {
    File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  auto glsl_vert = convertShader({
    .shaderSource = File("asset://shader/lighting.vert").read(),
    .shaderType = ShaderConverterStage::VERTEX,
    .shaderConverterType = ShaderConverterTarget::GLSL
  });

  auto glsl_frag = convertShader({
    .shaderSource = File("asset://shader/lighting.frag").read(),
    .shaderType = ShaderConverterStage::FRAGMENT,
    .shaderConverterType = ShaderConverterTarget::GLSL
  });

  std::cout << "============== glsl_vert ==============" << std::endl;
  std::cout << glsl_vert << std::endl;

  std::cout << "============== glsl_frag ==============" << std::endl;
  std::cout << glsl_frag << std::endl;

  auto msl_vert = convertShader({
    .shaderSource = File("asset://shader/lighting.vert").read(),
    .shaderType = ShaderConverterStage::VERTEX,
    .shaderConverterType = ShaderConverterTarget::MSL
  });

  auto msl_frag = convertShader({
    .shaderSource = File("asset://shader/lighting.frag").read(),
    .shaderType = ShaderConverterStage::FRAGMENT,
    .shaderConverterType = ShaderConverterTarget::MSL
  });

  std::cout << "============== msl_vert ==============" << std::endl;
  std::cout << msl_vert << std::endl;

  std::cout << "============== msl_frag ==============" << std::endl;
  std::cout << msl_frag << std::endl;

  return 0;
}
