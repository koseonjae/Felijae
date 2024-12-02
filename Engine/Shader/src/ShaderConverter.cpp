#include <Shader/ShaderConverter.h>

#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/MachineIndependent/localintermediate.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#include <spirv_msl.hpp>

#include <ostream>
#include <iostream>
#include <vector>

namespace {
std::vector<uint32_t> compileVulkanGlsl2Spirv(const std::string& glslShaderCode, goala::ShaderConverterStage shaderType) {
  std::vector<uint32_t> spirvData;

  EShLanguage stage = EShLangVertex;
  if (shaderType == goala::ShaderConverterStage::VERTEX)
    stage = EShLangVertex;
  else if (shaderType == goala::ShaderConverterStage::FRAGMENT)
    stage = EShLangFragment;
  else
    assert(false && "not supported");

  // GLSL 프래그먼트 쉐이더 생성
  glslang::TShader shader(stage);
  const char* shaderSource = glslShaderCode.c_str();
  shader.setStrings(&shaderSource, 1);
  shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_0);
  // shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientOpenGL, 450);

  // GLSL 코드 컴파일
  auto defaultRes = GetDefaultResources();
  if (!shader.parse(defaultRes, 450, ENoProfile, true, false, EShMsgDefault)) {
    std::cerr << "Failed to compile GLSL shader: " << shader.getInfoLog() << std::endl;
    assert(false && "Failed to compile GLSL shader");
  }

  // SPIR-V 바이너리로 변환
  glslang::TProgram program;
  program.addShader(&shader);
  if (!program.link(EShMsgDefault)) {
    std::cerr << "Failed to link GLSL shader: " << program.getInfoLog() << std::endl;
    assert(false && "Failed to link GLSL shader");
  }

  // SPIR-V 바이너리 생성
  glslang::SpvOptions spvOptions{};
  auto intermediate = program.getIntermediate(stage);

  // SPIR-V로 변환
  try {
    GlslangToSpv(*intermediate, spirvData, &spvOptions);
  }
  catch (const std::exception& e) {
    std::cerr << "Error converting GLSL to SPIR-V: " << e.what() << std::endl;
    assert(false && "Error converting GLSL to SPIR-V");
  }

  return spirvData;
}

std::string convertSpirv2glsl(const std::vector<uint32_t>& spirvData) {
  try {
    spirv_cross::CompilerGLSL compiler(spirvData);

    spirv_cross::CompilerGLSL::Options options = {
      .version = 300,
      .es = false
    };
    compiler.set_common_options(options);

    auto compiled = compiler.compile();
    assert(!compiled.empty() && "Compiled source is empty");
    return compiled;
  }
  catch (const spirv_cross::CompilerError& e) {
    std::cerr << "SPIRV Cross error: " << e.what() << std::endl;
    assert(false && "SPIRV Cross error");
  }
}

std::string convertSpirv2msl(const std::vector<uint32_t>& spirvData) {
  try {
    spirv_cross::CompilerMSL compiler(spirvData);

    spirv_cross::CompilerMSL::Options options = {
      .platform = spirv_cross::CompilerMSL::Options::Platform::macOS,
    };
    compiler.set_msl_options(options);

    auto compiled = compiler.compile();
    assert(!compiled.empty() && "Compiled source is empty");
    return compiled;
  }
  catch (const spirv_cross::CompilerError& e) {
    std::cerr << "SPIRV Cross error: " << e.what() << std::endl;
    assert(false && "SPIRV Cross error");
  }
}
} // namespace

namespace goala {
std::string convertShader(const ShaderConverterDesc& shaderConverterDesc) {
  static std::atomic<bool> initialized = false;
  bool expected = false;
  if (initialized.compare_exchange_strong(expected, true))
    glslang::InitializeProcess(); // todo: FinalizeProcess

  auto spirv = compileVulkanGlsl2Spirv(shaderConverterDesc.shaderSource, shaderConverterDesc.shaderType);
  std::string converted;
  if (shaderConverterDesc.shaderConverterType == ShaderConverterTarget::GLSL)
    converted = convertSpirv2glsl(spirv);
  else if (shaderConverterDesc.shaderConverterType == ShaderConverterTarget::MSL)
    converted = convertSpirv2msl(spirv);
  else
    assert(false && "Unsupported shader converter");
  return converted;
}
} // namespace goala
