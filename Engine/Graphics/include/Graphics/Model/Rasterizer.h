#pragma once

#include <tuple>

namespace goala {
class CommandEncoder;

struct Culling {
  enum class CullMode {
    Front,
    Back
  };

  enum class FrontFace {
    CCW,
    CW
  };

  bool enable = true;
  FrontFace frontFace = FrontFace::CCW;
  CullMode cullMode = CullMode::Back;
};

static_assert(std::is_trivially_copyable_v<Culling>);

struct Viewport {
  int minX = -1;
  int minY = -1;
  int width = -1;
  int height = -1;
  float minZ = -1.0f;
  float maxZ = -1.0f;
};

static_assert(std::is_trivially_copyable_v<Viewport>);

struct Rasterizer {
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion
  Culling culling{};
  Viewport viewport{};
};
} // namespace goala
