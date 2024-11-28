#pragma once

#include <memory>

namespace goala {
class Buffer;
class Fence;
class Pipeline;
class Texture;

struct BufferDescription;
struct FenceDescription;
struct PipelineDescription;
struct TextureDescription;

class Device {
public:
  virtual ~Device() = default;

  virtual std::shared_ptr<Pipeline> createPipeline(PipelineDescription desc) = 0;

  virtual std::shared_ptr<Buffer> createBuffer(BufferDescription desc) = 0;

  virtual std::shared_ptr<Texture> createTexture(TextureDescription desc) = 0;

  virtual std::shared_ptr<Fence> createFence(FenceDescription desc) = 0;
};
} // namespace goala
