#pragma once

#include "OpenGLProgram.h"
#include "OpenGLModel.h"

#include <glad/glad.h>

class Rasterizer;
class OpenGLPipeline;

class Culling {
  friend class Rasterizer;
 public:
  enum class CullMode {
    Front, Back
  };
  enum class FrontFace {
    CCW, CW
  };
  inline void enableCulling(bool enable) {
    m_enable = enable;
  }
  inline void setFrontFace(FrontFace frontFace) {
    m_frontFace = frontFace;
  }
  inline void setCullMode(CullMode mode) {
    m_cullMode = mode;
  }

 private:
  void bind();

 private:
  bool m_enable = true;
  FrontFace m_frontFace = FrontFace::CCW;
  CullMode m_cullMode = CullMode::Back;
};

class Viewport {
  friend class Rasterizer;
 public:
  void setViewport(int minX, int minY, int width, int height) {
    m_minX = minX;
    m_minY = minY;
    m_width = width;
    m_height = height;
  }
  void setDepth(float minZ, float maxZ) {
    m_minZ = minZ;
    m_maxZ = maxZ;
  }
 private:
  void bind();
 private:
  int m_minX = -1;
  int m_minY = -1;
  int m_width = -1;
  int m_height = -1;
  float m_minZ = 0;
  float m_maxZ = 1;
};

class Rasterizer {
  friend class OpenGLPipeline;
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion
 private:
  void bind();

 public:
  Culling culling;
  Viewport viewport;
};

class OutputMerger {
  friend class OpenGLPipeline;
  // depth test
  // alpha blending
  // color buffer
  // depth buffer
  // clear, clearColor?
 private:
  void bind();
};

class OpenGLPipeline {
 public:
  OpenGLPipeline() = default;
  ~OpenGLPipeline() noexcept = default;
  void bind();

 public:
  Rasterizer rasterizer;
  OutputMerger outputMerger;
};
