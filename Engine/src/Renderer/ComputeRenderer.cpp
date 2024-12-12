#include <Engine/Renderer/ComputeRenderer.h>
#include <Graphics/Model/ComputePipeline.h>
#include <Graphics/Model/ComputeCommandEncoder.h>
#include <Graphics/Model/CommandBuffer.h>

using namespace goala;

void ComputeRenderer::update() {
  if (m_pipelines.empty())
    return;
}

void ComputeRenderer::render(std::shared_ptr<CommandBuffer>& cmdBuf) {
  if (m_pipelines.empty())
    return;
  auto encoder = cmdBuf->createComputeCommandEncoder({});
  for (auto& pipeline : m_pipelines)
    encoder->encodeCompute(pipeline.get());
  encoder->endEncoding();
}

void ComputeRenderer::addPipeline(std::shared_ptr<ComputePipeline> pipeline) {
  m_pipelines.emplace_back(std::move(pipeline));
}
