#pragma once

#include <Engine/Renderer/Renderer.h>

#include <vector>

namespace goala {
class ComputePipeline;

class ComputeRenderer : public Renderer {
public:
  void update() override;

  void render(std::shared_ptr<CommandBuffer> cmdBuf) override;

  void addPipeline(std::shared_ptr<ComputePipeline> pipeline);

protected:
  std::vector<std::shared_ptr<ComputePipeline>> m_pipelines;
};
} // namespace goala
