#pragma once

#include <memory>

class Culling final {
 public:
  enum class CullMode {
    Front, Back
  };
  enum class FrontFace {
    CCW, CW
  };
  inline void enableCulling(bool enable) { m_enable = enable; }
  inline void setFrontFace(FrontFace frontFace) { m_frontFace = frontFace; }
  inline void setCullMode(CullMode mode) { m_cullMode = mode; }
  bool getEnable() const { return m_enable; };
  FrontFace getFrontFace() const { return m_frontFace; };
  CullMode getCullMode() const { return m_cullMode; }
  std::tuple<bool, FrontFace, CullMode> getCulling() const {
    return {m_enable, m_frontFace, m_cullMode};
  }

 protected:
  bool m_enable = true;
  FrontFace m_frontFace = FrontFace::CCW;
  CullMode m_cullMode = CullMode::Back;
};

class Viewport final {
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

  std::tuple<int, int, int, int, float, float> getViewPort() const {
    return {m_minX, m_minY, m_width, m_height, m_minZ, m_maxZ};
  }

 protected:
  int m_minX = -1;
  int m_minY = -1;
  int m_width = -1;
  int m_height = -1;
  float m_minZ = 0;
  float m_maxZ = 1;
};

class Rasterizer final {
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion

 public:
  void setCulling(Culling culling) { m_culling = culling; };
  void setViewport(Viewport viewPort) { m_viewport = viewPort; }
  const Culling &getCulling() const { return m_culling; }
  Culling &getCulling() { return m_culling; }
  const Viewport &getViewport() const { return m_viewport; }
  Viewport &getViewport() { return m_viewport; }

 private:
  Culling m_culling;
  Viewport m_viewport;
};

class OutputMerger final {
  friend class Pipeline;
  // depth test
  // alpha blending
  // color buffer
  // depth buffer
  // clear, clearColor?
  // todo: implement
};

class Pipeline {
 public:
  virtual ~Pipeline() = default;

  void setRasterizer(Rasterizer rasterizer) { m_rasterizer = std::move(rasterizer); }
  void setOutputMerger(OutputMerger output_merger) { m_outputMerger = std::move(output_merger); }
  const Rasterizer &getRasterizer() const { return m_rasterizer; }
  Rasterizer &getRasterizer() { return m_rasterizer; }
  const OutputMerger &getOutputMerger() const { return m_outputMerger; }
  OutputMerger &getOutputMerger() { return m_outputMerger; }

  virtual void bind() = 0;

 protected:
  Rasterizer m_rasterizer;
  OutputMerger m_outputMerger;
};