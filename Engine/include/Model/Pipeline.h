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
  void enableCulling(bool enable) { m_enable = enable; }
  void setFrontFace(FrontFace frontFace) { m_frontFace = frontFace; }
  void setCullMode(CullMode mode) { m_cullMode = mode; }

  bool getEnable() const { return m_enable; };
  FrontFace getFrontFace() const { return m_frontFace; };
  CullMode getCullMode() const { return m_cullMode; }
  auto getVariables() const { return std::make_tuple(m_enable, m_frontFace, m_cullMode); }

 protected:
  bool m_enable = true;
  FrontFace m_frontFace = FrontFace::CCW;
  CullMode m_cullMode = CullMode::Back;
};
static_assert(std::is_trivially_copyable_v<Culling>);

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
  auto getVariables() const {
    return std::make_tuple(m_minX, m_minY, m_width, m_height, m_minZ, m_maxZ);
  }

 private:
  int m_minX = -1;
  int m_minY = -1;
  int m_width = -1;
  int m_height = -1;
  float m_minZ = 0.0f;
  float m_maxZ = 1.0f;
};
static_assert(std::is_trivially_copyable_v<Viewport>);

class Rasterizer final {
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion

 public:
  void setCulling(const Culling& culling) { m_culling = culling; };
  void setViewport(const Viewport& viewPort) { m_viewport = viewPort; }

  Culling& getCulling() { return m_culling; }
  const Culling& getCulling() const { return m_culling; }
  Viewport& getViewport() { return m_viewport; }
  const Viewport& getViewport() const { return m_viewport; }

 private:
  Culling m_culling;
  Viewport m_viewport;
};
static_assert(std::is_trivially_copyable_v<Rasterizer>);

class DepthTest final {
 public:
  enum class DepthTestFunc {
    Less,
  };
  void setEnable(bool enable) { DepthTest::m_enable = enable; }
  void setDepthFunc(DepthTestFunc depthFunc) { m_depthFunc = depthFunc; }
  void updateDepthMask(bool update) { m_updateDepthMask = update; };
  bool getEnable() const { return m_enable; }
  DepthTestFunc getDepthFunc() const { return m_depthFunc; }
  auto getVariables() const {
    return std::make_tuple(m_enable, m_depthFunc, m_updateDepthMask);
  }

 private:
  bool m_enable = true;
  DepthTestFunc m_depthFunc = DepthTestFunc::Less;
  bool m_updateDepthMask = true;
};
static_assert(std::is_trivially_copyable_v<DepthTest>);

class AlphaBlend final {
 public:
  enum class BlendFunc {
    SRC_ALPHA, ONE_MINUS_SRC_ALPHA,
  };
  enum class BlendEquation {
    Add,
  };

 public:
  void setEnable(bool enable) { m_enable = enable; }
  void setFragmentBlendFunc(BlendFunc fragmentBlendFunc) { m_fragmentBlendFunc = fragmentBlendFunc; }
  void setPixelBlendFunc(BlendFunc pixelBlendFunc) { m_pixelBlendFunc = pixelBlendFunc; }
  void setBlendEquation(BlendEquation blendEquation) { m_blendEquation = blendEquation; }

  bool getEnable() const { return m_enable; }
  BlendFunc getFragmentBlendFunc() const { return m_fragmentBlendFunc; }
  BlendFunc getPixelBlendFunc() const { return m_pixelBlendFunc; }
  BlendEquation getBlendEquation() const { return m_blendEquation; }
  auto getVariables() const {
    return std::make_tuple(m_enable, m_fragmentBlendFunc, m_pixelBlendFunc, m_blendEquation);
  }

 private:
  bool m_enable = true;
  BlendFunc m_fragmentBlendFunc = BlendFunc::SRC_ALPHA;
  BlendFunc m_pixelBlendFunc = BlendFunc::ONE_MINUS_SRC_ALPHA;
  BlendEquation m_blendEquation = BlendEquation::Add;
};
static_assert(std::is_trivially_copyable_v<AlphaBlend>);

class OutputMerger final {
  // depth test
  // alpha blending
 public:
  void setDepthTest(const DepthTest& depthTest) { m_depthTest = depthTest; }
  void setAlphaBlend(const AlphaBlend& alphaBlend) { m_alphaBlend = alphaBlend; }

  const DepthTest& getDepthTest() const { return m_depthTest; }
  DepthTest& getDepthTest() { return m_depthTest; }
  const AlphaBlend& getAlphaBlend() const { return m_alphaBlend; }
  AlphaBlend& getAlphaBlend() { return m_alphaBlend; }

 private:
  DepthTest m_depthTest;
  AlphaBlend m_alphaBlend;
};
static_assert(std::is_trivially_copyable_v<OutputMerger>);

class Pipeline {
 public:
  virtual ~Pipeline() = default;

 public:
  virtual void bind() = 0;

 public:
  void setRasterizer(const Rasterizer& rasterizer) { m_rasterizer = rasterizer; }
  void setOutputMerger(const OutputMerger& output_merger) { m_outputMerger = output_merger; }

  const Rasterizer& getRasterizer() const { return m_rasterizer; }
  Rasterizer& getRasterizer() { return m_rasterizer; }
  const OutputMerger& getOutputMerger() const { return m_outputMerger; }
  OutputMerger& getOutputMerger() { return m_outputMerger; }

 protected:
  Rasterizer m_rasterizer;
  OutputMerger m_outputMerger;
};
