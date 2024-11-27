#pragma once

#include <memory>

namespace goala {

class Buffer;
class Pipeline;

struct BufferDescription;
struct PipelineDescription;

class Device {
 public:
  virtual ~Device() = default;

  virtual std::shared_ptr<Pipeline> createPipeline(const PipelineDescription& desc) = 0;

  virtual std::shared_ptr<Buffer> createBuffer(const BufferDescription& desc) = 0;
};
} // namespace goala
