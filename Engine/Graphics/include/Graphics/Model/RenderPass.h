#pragma once

#include <Graphics/Model/Texture.h>

#include <vector>

namespace goala {
enum class AttachmentType {
  Color,
  Depth,
  Stencil,
  Undefined
};

enum class LoadFunc {
  Clear,
  DontCare,
  Undefined
};

enum class StoreFunc {
  Store,
  DontCare,
  Undefined
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
  virtual ~RenderPass() = default;
};
} // namespace goala
