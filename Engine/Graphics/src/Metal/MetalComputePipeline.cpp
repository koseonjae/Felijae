#include <Graphics/Metal/MetalComputePipeline.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalShader.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Metal/MetalTexture.h>

#include <Metal/Metal.hpp>

#include "Graphics/Metal/MetalVertexBuffer.h"

using namespace goala;

MetalComputePipeline::MetalComputePipeline(MetalDevice* device, ComputePipelineDescription desc)
  : ComputePipeline(std::move(desc))
  , m_device(device) {
  auto pipelineDesc = MTL::ComputePipelineDescriptor::alloc()->init(); // 기존 demo에선 이걸 안쓰고 metal shader function만을 넘겼음
  _initializeBuffer(pipelineDesc);
  _initializeShader(pipelineDesc);
  _initializePipeline(pipelineDesc);
}

// todo: output texture를 받는 방법 정리
void MetalComputePipeline::encode(MTL::ComputeCommandEncoder* computeEncoder) {
  computeEncoder->setComputePipelineState(m_pipelineState.get());

  int bufIdx = 0;
  computeEncoder->setBuffer(m_buffer.get(), 0, bufIdx++);

  for (auto& uniform : m_desc.uniforms) {
    const auto [valuePtr, valueSize] = getUniformAddress(uniform);
    computeEncoder->setBytes(valuePtr, valueSize, bufIdx++);
  }

  auto& textures = m_desc.textures;
  for (int i = 0; i < textures.size(); i++)
    computeEncoder->setTexture(SAFE_DOWN_CAST(MetalTexture*, textures[i].get())->getTextureHandle(), i);

  // todo: research better thread dispatch rule
  // https://developer.apple.com/documentation/metal/compute_passes/calculating_threadgroup_and_grid_sizes
  MTL::Size threadPerGrid(m_bufferWidth, m_bufferHeight, 1);
  auto threadGroupWidth = m_pipelineState->threadExecutionWidth();
  auto threadGroupHeight = m_pipelineState->maxTotalThreadsPerThreadgroup() / threadGroupWidth;
  MTL::Size threadGroups(threadGroupWidth, threadGroupHeight, 1);
  computeEncoder->dispatchThreadgroups(threadPerGrid, threadGroups);

  computeEncoder->endEncoding();
}

void MetalComputePipeline::_initializeBuffer(MTL::ComputePipelineDescriptor* pipelineDesc) {
  // todo: use ComputeBuffer
  m_buffer = makeMetalRef(m_device->getMTLDevice()->newBuffer(m_desc.buffer.data.data(), m_desc.buffer.data.size() * sizeof(float), MTL::ResourceStorageModeShared));
  m_bufferWidth = m_desc.buffer.width;
  m_bufferHeight = m_desc.buffer.height;
}

void MetalComputePipeline::_initializeShader(MTL::ComputePipelineDescriptor* pipelineDesc) {
  m_shader = m_device->createShader(m_desc.shader);
}

void MetalComputePipeline::_initializePipeline(MTL::ComputePipelineDescriptor* pipelineDesc) {
  auto metalShader = SAFE_DOWN_CAST(MetalShader*, m_shader.get());
  NS::Error* err = nil;
  m_pipelineState = makeMetalRef(m_device->getMTLDevice()->newComputePipelineState(metalShader->getFunction(), &err));
  assert(!err && "Failed to create pipeline");
}
