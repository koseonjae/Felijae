#pragma once

#include <Graphics/Model/Texture.h>

#include <vector>

namespace larco {
enum class AttachmentType {
  Undefined,
  Color,
  Depth,
  Stencil,
};

enum class LoadFunc {
  Undefined,
  Load,
  Clear,
  DontCare,
};

enum class StoreFunc {
  Undefined,
  Store,
  DontCare,
};

struct ClearColor {
  float r = -1.0f;
  float g = -1.0f;
  float b = -1.0f;
  float a = -1.0f;
};

struct ClearDepth {
  float depth = -1.0f;
};

struct ClearStencil {
  int s = INT_MAX;
};

struct Attachment {
  AttachmentType type = AttachmentType::Undefined;
  LoadFunc loadFunc = LoadFunc::Undefined;
  StoreFunc storeFunc = StoreFunc::Undefined;
  std::variant<ClearColor, ClearDepth, ClearStencil> clear;
  std::shared_ptr<Texture> texture;
};

struct RenderPassDescription {
  std::vector<Attachment> attachments;
};

class RenderPass {
public:
  RenderPass(RenderPassDescription desc) : m_desc(std::move(desc)) {}
  virtual ~RenderPass() = default;

  const RenderPassDescription& getDescription() const { return m_desc; }
protected:
  RenderPassDescription m_desc{};
};
} // namespace larco
