#pragma once

#include <tuple>

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

class Rasterizer {
  // primitive assembly
  // clipping
  // perspective division
  // culling
  // viewport transform
  // scan conversion

 public:
  virtual void bind() = 0;

 public:
  virtual ~Rasterizer() = default;
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
