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
  computeEncoder->setBuffer(m_buffer.get(), 0, 0);

  auto& uniforms = m_desc.uniforms;
  for (int i = 0 ; i < uniforms.size(); ++i) {
    const auto [valuePtr, valueSize] = getUniformAddress(uniforms[i]);
    computeEncoder->setBytes(valuePtr, valueSize, i);
  }

  auto& textures = m_desc.textures;
  for (int i = 0; i < textures.size(); i++)
    computeEncoder->setTexture(SAFE_DOWN_CAST(MetalTexture*, textures[i].get())->getTextureHandle(), i);

  constexpr int threadGroupSize = 64;
  MTL::Size threadGroupSize1D(threadGroupSize, 1, 1);
  MTL::Size threadGroups1D((m_desc.buffer.data.size() + threadGroupSize - 1) / threadGroupSize, 1, 1);
  computeEncoder->dispatchThreadgroups(threadGroups1D, threadGroupSize1D);

  computeEncoder->endEncoding();
}

void MetalComputePipeline::_initializeBuffer(MTL::ComputePipelineDescriptor* pipelineDesc) {
  // todo: use ComputeBuffer
  m_buffer = makeMetalRef(m_device->getMTLDevice()->newBuffer(m_desc.buffer.data.data(), m_desc.buffer.data.size() * sizeof(float), MTL::ResourceStorageModeShared));
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
