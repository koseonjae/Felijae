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

class Rasterizer {
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion

public:
  virtual void bind(void* descriptor) = 0;
public:
  virtual ~Rasterizer() = default;

  void setCulling(const Culling& culling) { m_culling = culling; };
  void setViewport(const Viewport& viewPort) { m_viewport = viewPort; }

  const Culling& getCulling() const { return m_culling; }
  const Viewport& getViewport() const { return m_viewport; }

private:
  Culling m_culling;
  Viewport m_viewport;
};
} // namespace goala
