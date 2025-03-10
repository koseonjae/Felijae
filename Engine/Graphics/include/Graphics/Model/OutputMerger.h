#pragma once

#include <tuple>

namespace larco {
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

struct OutputMerger {
  DepthTest depthTest{};
  AlphaBlend alphaBlend{};
};
} // namespace larco
