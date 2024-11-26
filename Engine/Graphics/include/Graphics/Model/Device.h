#pragma once

#include <memory>

namespace goala {

class Pipeline;
struct PipelineDescription;

class Device {
 public:
  virtual ~Device() = default;

  virtual std::shared_ptr<Pipeline> createPipeline(const PipelineDescription& desc) = 0;
};
} // namespace goala
