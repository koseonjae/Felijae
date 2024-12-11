#include <Graphics/Metal/MetalComputePipeline.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Metal/MetalVertexBuffer.h>
#include <Base/Utility/TypeCast.h>

#include <Metal/Metal.hpp>

using namespace goala;

MetalComputePipeline::MetalComputePipeline(MetalDevice* device, ComputePipelineDescription desc)
  : ComputePipeline(std::move(desc))
  , m_device(device) {
  auto pipelineDesc = MTL::ComputePipelineDescriptor::alloc()->init(); // 기존 demo에선 이걸 안쓰고 metal shader function만을 넘겼음
  _initializeShader(pipelineDesc);
  _initializePipeline(pipelineDesc);
}

// todo: output texture를 받는 방법 정리
void MetalComputePipeline::encode(MTL::ComputeCommandEncoder* computeEncoder) {
  computeEncoder->setComputePipelineState(m_pipelineState.get());

  int bufIdx = 0;

  for (auto& buffer : m_desc.buffers)
    computeEncoder->setBuffer(buffer.get(), 0, bufIdx++);

  for (auto& uniform : m_desc.uniforms) {
    const auto [valuePtr, valueSize] = getUniformAddress(uniform);
    computeEncoder->setBytes(valuePtr, valueSize, bufIdx++);
  }

  auto& textures = m_desc.textures;
  for (int i = 0; i < textures.size(); i++) {
    MTL::Texture* handle = nullptr;
    std::visit([&](auto& texture) {
      using T = std::decay_t<decltype(texture)>;
      if constexpr (std::is_same_v<T, MTL::Texture*>)
        handle = texture;
      else if constexpr (std::is_same_v<T, std::shared_ptr<Texture>>)
        handle = SAFE_DOWN_CAST(MetalTexture*, texture.get())->getTextureHandle();
      else
        assert(false && "Unsupported type");
    }, textures[i]);
    computeEncoder->setTexture(handle, i);
  }

  // todo: research better thread dispatch rule
  // https://developer.apple.com/documentation/metal/compute_passes/calculating_threadgroup_and_grid_sizes

  MTL::Size threadPerGrid{};
  MTL::Size threadGroups{};
  assert(!m_desc.threadSize.empty() && "Invalid thread size");
  if (m_desc.threadSize.size() == 1) {
    assert(false && "Need to support");
  }
  else if (m_desc.threadSize.size() == 2) {
    int threadWidth = m_desc.threadSize[0];
    int threadHeight = m_desc.threadSize[1];
    threadPerGrid = MTL::Size(threadWidth, threadHeight, 1);

    auto threadGroupWidth = m_pipelineState->threadExecutionWidth();
    auto threadGroupHeight = m_pipelineState->maxTotalThreadsPerThreadgroup() / threadGroupWidth;
    threadGroups = MTL::Size(threadGroupWidth, threadGroupHeight, 1);
  }
  else
    assert(false && "3D thread size is not supported");

  computeEncoder->dispatchThreadgroups(threadPerGrid, threadGroups);

  computeEncoder->endEncoding();
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
