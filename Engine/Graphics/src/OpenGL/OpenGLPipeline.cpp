#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLProgram.h>
#include <Graphics/OpenGL/OpenGLDevice.h>
#include <ShaderConverter/ShaderConverter.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>

#include <OpenGL/gl3.h>

using namespace goala;

OpenGLPipeline::OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc))
  , m_device(device){
  _initializeProgram();
  _initializeVertexBuffer();
}

void OpenGLPipeline::render() {
  assert(m_program && m_vertexBuffer && "There is empty pipeline");
  _bindViewport();
  _bindCulling();
  _bindDepthTest();
  _bindAlphaBlending();
  m_program->bind(m_desc.uniforms.get());
  auto glVertexBuffer = SAFE_DOWN_CAST(OpenGLBuffer*, m_vertexBuffer.get());
  glVertexBuffer->bind();
  glVertexBuffer->draw();
}

void OpenGLPipeline::_bindCulling() {
  const auto& culling = m_desc.rasterizer.culling;

  if (!culling.enable) {
    glDisable(GL_CULL_FACE);
    return;
  }
  glEnable(GL_CULL_FACE);

  if (culling.frontFace == Culling::FrontFace::CCW)
    glFrontFace(GL_CCW);
  else
    glFrontFace(GL_CW);

  if (culling.cullMode == Culling::CullMode::Back)
    glCullFace(GL_BACK);
  else
    glCullFace(GL_FRONT);
}

void OpenGLPipeline::_bindViewport() {
  const auto& viewport = m_desc.rasterizer.viewport;

  assert(viewport.minX >= 0 && viewport.minY >= 0 && "Invalid viewport minX, minY");
  assert(viewport.width > 0 && viewport.height > 0 && "Invalid viewport width, height");
  glViewport(viewport.minX, viewport.minY, viewport.width, viewport.height);

  assert(0.f <= viewport.minZ && viewport.minZ <= 1.f && "Invalid viewport minZ");
  assert(0.f <= viewport.maxZ && viewport.maxZ <= 1.f && "Invalid viewport maxZ");
  glDepthRangef(viewport.minZ, viewport.maxZ);
}

void OpenGLPipeline::_bindDepthTest() {
  const auto& depthTest = m_desc.outputMerger.depthTest;

  if (!depthTest.enable) {
    glDisable(GL_DEPTH_TEST);
    return;
  }
  glEnable(GL_DEPTH_TEST);

  if (depthTest.updateDepthMask)
    glDepthMask(GL_TRUE);
  else
    glDepthMask(GL_FALSE);

  GLenum depthFunc = GL_NONE;
  switch (depthTest.depthFunc) {
    case DepthTest::DepthTestFunc::Less: {
      depthFunc = GL_LESS;
      break;
    }
    default: {
      depthFunc = GL_NONE;
    }
  }
  assert(depthFunc != GL_NONE && "[OpenGLPipeline] Invalid depth function.");
  glDepthFunc(depthFunc);
}

void OpenGLPipeline::_bindAlphaBlending() {
  const auto& alphaBlend = m_desc.outputMerger.alphaBlend;

  if (!alphaBlend.enable) {
    glDisable(GL_BLEND);
    return;
  }
  glEnable(GL_BLEND);

  auto getBlendFunc = [](AlphaBlend::BlendFunc blendFunc) {
    switch (blendFunc) {
      case AlphaBlend::BlendFunc::SRC_ALPHA:
        return GL_SRC_ALPHA;
      case AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
      default:
        return GL_NONE;
    }
  };
  GLenum fragBlendFunc = getBlendFunc(alphaBlend.fragmentBlendFunc);
  assert(fragBlendFunc != GL_NONE && "[OpenGLPipeline] Invalid frag blend function.");
  GLenum pixelBlendFunc = getBlendFunc(alphaBlend.pixelBlendFunc);
  assert(pixelBlendFunc != GL_NONE && "[OpenGLPipeline] Invalid pixel blend function.");
  glBlendFunc(fragBlendFunc, pixelBlendFunc);

  GLenum blendEquation = GL_NONE;
  switch (alphaBlend.blendEquation) {
    case AlphaBlend::BlendEquation::Add: {
      blendEquation = GL_FUNC_ADD;
      break;
    }
    default:
      blendEquation = GL_NONE;
  }
  assert(blendEquation != GL_NONE && "[OpenGLPipeline] Invalid blend equation.");
  glBlendEquation(blendEquation);
}

void OpenGLPipeline::_initializeProgram() {
  std::string vertexShaderSrc;
  std::string fragShaderSrc;
  for (const auto& shader : m_desc.shaders) {
    if (shader.type == ShaderType::VERTEX) {
      vertexShaderSrc = convertShader({
        .shaderSource = shader.source,
        .shaderType = getShaderConverterType(shader.type),
        .shaderConverterType = ShaderConverterTarget::GLSL
      });
    }
    else if (shader.type == ShaderType::FRAGMENT)
      fragShaderSrc = convertShader({
        .shaderSource = shader.source,
        .shaderType = getShaderConverterType(shader.type),
        .shaderConverterType = ShaderConverterTarget::GLSL
      });
    else
      assert(false && "Invalid shader type");
  }
  m_program = std::make_shared<OpenGLProgram>(vertexShaderSrc, fragShaderSrc);
}

void OpenGLPipeline::_initializeVertexBuffer() {
  m_vertexBuffer = m_device->createBuffer(m_desc.vertexBuffer);
}
