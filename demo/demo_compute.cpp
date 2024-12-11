#include <Metal/Metal.hpp>
#include <Base/Utility/FileReader.h>
#include <Base/File/File.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Metal/MetalComputePipeline.h>

#include <iostream>
#include <vector>

using namespace goala;

namespace {
constexpr int width = 4;
constexpr int height = 10;
constexpr int totalElements = width * height;
}

std::shared_ptr<ComputePipeline> createBuf2TexturePipeline(MetalDevice* device, MetalRef<MTL::Buffer> inputBuffer, std::shared_ptr<Texture> outputTexture) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/computePipeline0_buf2tex.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {inputBuffer},
    .threadSize = {width, height},
    .textures = {outputTexture}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

std::shared_ptr<ComputePipeline> createTexture2TexturePipeline(MetalDevice* device, std::shared_ptr<Texture> inputTexture, std::shared_ptr<Texture> outputTexture) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/computePipeline1_tex2tex.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {},
    .threadSize = {width, height},
    .textures = {inputTexture, outputTexture},
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

std::shared_ptr<ComputePipeline> createTexture2BufferPipeline(MetalDevice* device, std::shared_ptr<Texture> inputTexture, MetalRef<MTL::Buffer> outputBuffer) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/computePipeline2_tex2buf.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {outputBuffer},
    .threadSize = {width, height},
    .textures = {inputTexture}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

std::shared_ptr<ComputePipeline> createBuffer2BufferPipeline(MetalDevice* device, MetalRef<MTL::Buffer> inputBuffer, MetalRef<MTL::Buffer> outputBuffer) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/computePipeline3_buf2buf.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {inputBuffer, outputBuffer},
    .threadSize = {width, height},
    .textures = {}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

int main() {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  auto device = std::make_unique<MetalDevice>();
  assert(device && "Failed to create device");

  // Pipeline 0

  std::vector<float> inputData(totalElements);
  for (size_t i = 0; i < totalElements; ++i)
    inputData[i] = static_cast<float>(i);
  auto pipeline0_inputBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(inputData.data(), inputData.size() * sizeof(float), MTL::ResourceStorageModeShared));

  auto pipeline0outputTexture = device->createTexture({
    .imageData = {
      .pixel = {},
      .width = width,
      .height = height,
      .channels = {},
      .pixelFormat = {}
    },
    .sampler = {},
    .usage = TextureUsage::WRITE | TextureUsage::READ,
    .storage = TextureStorage::SHARED,
    .loadType = TextureLoadType::EAGER,
    .pipeline = TexturePipeline::COMPUTE,
    .textureFormat = ImageFormat::Float32
  });
  auto pipeline0_buf2tex = createBuf2TexturePipeline(device.get(), pipeline0_inputBuffer, pipeline0outputTexture);

  // Pipeline 1

  auto pipeline1outputTexture = device->createTexture({
    .imageData = {
      .pixel = {},
      .width = width,
      .height = height,
      .channels = {},
      .pixelFormat = {}
    },
    .sampler = {},
    .usage = TextureUsage::WRITE,
    .storage = TextureStorage::SHARED,
    .loadType = TextureLoadType::EAGER,
    .pipeline = TexturePipeline::COMPUTE,
    .textureFormat = ImageFormat::Float32
  });

  auto pipeline1_tex2tex = createTexture2TexturePipeline(device.get(), pipeline0outputTexture, pipeline1outputTexture);

  // Pipeline 2

  auto pipeline2_mtlBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(totalElements * sizeof(float), MTL::ResourceStorageModeShared));
  auto pipeline2_tex2buf = createTexture2BufferPipeline(device.get(), pipeline1outputTexture, pipeline2_mtlBuffer);

  // Pipeline 3

  auto pipeline3_mtlBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(totalElements * sizeof(float), MTL::ResourceStorageModeShared));
  auto pipeline3_buf2buf = createBuffer2BufferPipeline(device.get(), pipeline2_mtlBuffer, pipeline3_mtlBuffer);

  // Pipeline vector

  std::vector pipelines = {pipeline0_buf2tex, pipeline1_tex2tex, pipeline2_tex2buf, pipeline3_buf2buf};

  // Encoding

  auto commandQueue = device->createCommandQueue({});
  auto commandBuffer = commandQueue->createCommandBuffer({});
  auto mtlCmdBuffer = SAFE_DOWN_CAST(MetalCommandBuffer*, commandBuffer.get());
  for (auto& pipeline : pipelines) {
    auto computeEncoder = mtlCmdBuffer->getCommandBuffer()->computeCommandEncoder();
    SAFE_DOWN_CAST(MetalComputePipeline*, pipeline.get())->encode(computeEncoder);
  }
  commandBuffer->commit();
  commandBuffer->waitUntilCompleted();

  // Print output

  std::array<float, totalElements> outputBuffer{};

  // texture to array
  // MTL::Region region = MTL::Region::Make2D(0, 0, width, height);
  // SAFE_DOWN_CAST(MetalTexture*, pipeline1outputTexture.get())->getTextureHandle()->getBytes(outputBuffer.data(), width * sizeof(float), region, 0);

  // buffer to array
  std::memcpy(outputBuffer.data(), pipeline3_mtlBuffer->contents(), sizeof(float) * totalElements);

  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      size_t index = y * width + x;
      std::cout << outputBuffer[index] << "\t";
    }
    std::cout << std::endl;
  }

  return 0;
}
