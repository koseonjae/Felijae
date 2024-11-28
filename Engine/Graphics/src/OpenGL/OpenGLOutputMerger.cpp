#include <Graphics/OpenGL/OpenGLOutputMerger.h>
#include <OpenGL/gl3.h>

#include <cassert>

using namespace goala;

void OpenGLOutputMerger::bind(void* descriptor) {
  _bindDepthTest();
  _bindAlphaBlending();
}

void OpenGLOutputMerger::encode(CommandEncoder* encoder) {}

void OpenGLOutputMerger::_bindDepthTest() {
  const auto& depthTest = getDepthTest();

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

void OpenGLOutputMerger::_bindAlphaBlending() {
  const auto& alphaBlend = getAlphaBlend();

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
