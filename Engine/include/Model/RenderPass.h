#pragma once

#include <Model/Texture.h>

#include <vector>

enum class AttachmentType {
  Color, Depth, Stencil, Undefined
};

enum class LoadFunc {
  Clear, DontCare, Undefined
};

enum class StoreFunc {
  Store, DontCare, Undefined
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
  std::shared_ptr<Texture> texture;
  std::variant<ClearColor, ClearDepth, ClearStencil> clear;
  LoadFunc load = LoadFunc::Undefined;
  StoreFunc store = StoreFunc::Undefined;
  AttachmentType type = AttachmentType::Undefined;
  auto getVariables() const { return std::make_tuple(type, load, store, clear); }
};

class RenderPass {
 public:
  virtual ~RenderPass() = default;

  virtual void bind() = 0;

  virtual void render() = 0;

  void setAttachments(std::vector<Attachment> attachments) {
    m_attachments = std::move(attachments);
    m_updated = true;
  }

 protected:
  std::vector<Attachment> m_attachments;
  bool m_updated = false;
};
