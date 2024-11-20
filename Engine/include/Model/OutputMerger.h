#pragma once

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
