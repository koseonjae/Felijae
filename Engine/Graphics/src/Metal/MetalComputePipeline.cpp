#include <Graphics/Metal/MetalComputePipeline.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalShader.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Metal/MetalTexture.h>

#include <Metal/Metal.hpp>

#include "Graphics/Metal/MetalBuffer.h"

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
void MetalComputePipeline::encode(MTL::ComputeCommandEncoder* computeEncoder, Texture* texture) {
  computeEncoder->setComputePipelineState(m_pipelineState.get());
  computeEncoder->setBuffer(m_buffer.get(), 0, 0);
  computeEncoder->setTexture(SAFE_DOWN_CAST(MetalTexture*, texture)->getTextureHandle(), 0);

  computeEncoder->setBytes(&m_bufferWidth, sizeof(m_bufferWidth), 1);
  computeEncoder->setBytes(&m_bufferHeight, sizeof(m_bufferHeight), 2);

  // 스레드 그룹 설정
  constexpr int threadGroupWidth = 8;
  constexpr int threadGroupHeight = 8;
  MTL::Size threadGroupSize(threadGroupWidth, threadGroupHeight, 1);
  MTL::Size threadGroups((m_bufferWidth + (threadGroupWidth - 1)) / threadGroupWidth, (m_bufferHeight + (threadGroupHeight - 1)) / threadGroupHeight, 1);
  computeEncoder->dispatchThreadgroups(threadGroups, threadGroupSize);

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
