#pragma once

#include <tuple>

namespace goala {
class CommandEncoder;

struct DepthTest {
  enum class DepthTestFunc {
    Less,
  };

  bool enable = true;
  DepthTestFunc depthFunc = DepthTestFunc::Less;
  bool updateDepthMask = true;
};

static_assert(std::is_trivially_copyable_v<DepthTest>);

struct AlphaBlend {
  enum class BlendFunc {
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
  };

  enum class BlendEquation {
    Add,
  };

  bool enable = true;
  BlendFunc fragmentBlendFunc = BlendFunc::SRC_ALPHA;
  BlendFunc pixelBlendFunc = BlendFunc::ONE_MINUS_SRC_ALPHA;
  BlendEquation blendEquation = BlendEquation::Add;
};

static_assert(std::is_trivially_copyable_v<AlphaBlend>);

class OutputMerger {
public:
  virtual void bind(void* descriptor) = 0;
  virtual void encode(CommandEncoder* encoder) = 0;

public:
  virtual ~OutputMerger() = default;

  void setDepthTest(const DepthTest& depthTest) { m_depthTest = depthTest; }
  void setAlphaBlend(const AlphaBlend& alphaBlend) { m_alphaBlend = alphaBlend; }

  const DepthTest& getDepthTest() const { return m_depthTest; }
  const AlphaBlend& getAlphaBlend() const { return m_alphaBlend; }

private:
  DepthTest m_depthTest;
  AlphaBlend m_alphaBlend;
};
} // namespace goala
