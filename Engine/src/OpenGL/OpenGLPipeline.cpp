#include <OpenGL/OpenGLPipeline.h>

#include <OpenGL/gl3.h>
#include <cassert>

void OpenGLPipeline::update() {
}

void OpenGLPipeline::render() {
  m_renderPass->bind();
  _bindRasterizer();
  _bindOutputMerger();
  m_program->bind();
  m_buffer->bind();
  m_buffer->draw();
}

void OpenGLPipeline::_bindRasterizer() {
  _bindCulling();
  _bindViewport();
}

void OpenGLPipeline::_bindOutputMerger() {
  _bindDepthTest();
  _bindAlphaBlending();
}

void OpenGLPipeline::_bindCulling() {
  auto [enable, frontFace, cullMode] = m_rasterizer.getCulling().getVariables();
  if (!enable) {
    glDisable(GL_CULL_FACE);
    return;
  }

  glEnable(GL_CULL_FACE);

  if (frontFace == Culling::FrontFace::CCW)
    glFrontFace(GL_CCW);
  else
    glFrontFace(GL_CW);

  if (cullMode == Culling::CullMode::Back)
    glCullFace(GL_BACK);
  else
    glCullFace(GL_FRONT);
}

void OpenGLPipeline::_bindViewport() {
  auto [minX, minY, width, height, minZ, maxZ] = m_rasterizer.getViewport().getVariables();
  assert(minX >= 0 && minY >= 0 && width > 0 && height > 0 && "Invalid viewport");
  glViewport(minX, minY, width, height);
  assert(0 <= minZ && minZ <= 1 && 0 <= maxZ && maxZ <= 1 && "Invalid viewport depth value");
  glDepthRangef(minZ, maxZ);
}

void OpenGLPipeline::_bindDepthTest() {
  auto [enable, pipelineDepthFunc, updateDepthMask] = m_outputMerger.getDepthTest().getVariables();
  if (!enable) {
    glDisable(GL_DEPTH_TEST);
    return;
  }
  glEnable(GL_DEPTH_TEST);

  if (updateDepthMask)
    glDepthMask(GL_TRUE);
  else
    glDepthMask(GL_FALSE);

  GLenum depthFunc = GL_NONE;
  switch (pipelineDepthFunc) {
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
  auto [enable, fragFunc, pixelFunc, equation] = m_outputMerger.getAlphaBlend().getVariables();
  if (!enable) {
    glDisable(GL_BLEND);
    return;
  }
  glEnable(GL_BLEND);

  auto getBlendFunc = [](AlphaBlend::BlendFunc blendFunc) {
    switch (blendFunc) {
      case AlphaBlend::BlendFunc::SRC_ALPHA: return GL_SRC_ALPHA;
      case AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
      default: return GL_NONE;
    }
  };
  GLenum fragBlendFunc = getBlendFunc(fragFunc);
  assert(fragBlendFunc != GL_NONE && "[OpenGLPipeline] Invalid frag blend function.");
  GLenum pixelBlendFunc = getBlendFunc(pixelFunc);
  assert(pixelBlendFunc != GL_NONE && "[OpenGLPipeline] Invalid pixel blend function.");
  glBlendFunc(fragBlendFunc, pixelBlendFunc);

  GLenum blendEquation = GL_NONE;
  switch (equation) {
    case AlphaBlend::BlendEquation::Add: {
      blendEquation = GL_FUNC_ADD;
      break;
    }
    default:blendEquation = GL_NONE;
  }
  assert(blendEquation != GL_NONE && "[OpenGLPipeline] Invalid blend equation.");
  glBlendEquation(blendEquation);
}
