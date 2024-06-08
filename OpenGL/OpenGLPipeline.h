#pragma once

#include "OpenGLProgram.h"
#include "Model.h"

#include <glad/glad.h>

class Culling {
 public:
  void bind();
  void enableCulling(bool enable) {
    m_enable = enable;
  }
  void setFrontFace(bool ccw) {
    m_ccw = ccw;
  }
  void cullBackFace(bool backFace) {
    m_cullBackFace = backFace;
  }

 private:
  bool m_enable = true;
  bool m_ccw = true;
  bool m_cullBackFace = true;

};

class Viewport {
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
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion
 public:
  void bind();

 public:
  Culling culling;
  Viewport viewport;
};

class OutputMerger {
  // depth test
  // alpha blending
  // color buffer
  // depth buffer
  // clear, clearColor?
 public:
  void bind();
};

class OpenGLPipeline {
 public:
  OpenGLPipeline() = default;
  ~OpenGLPipeline() noexcept = default;
  void bind();
  void draw();

 public:
  Rasterizer rasterizer;
  OutputMerger outputMerger;
  OpenGLProgram program;
  Model model;
};
