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

std::shared_ptr<ComputePipeline> createBuf2TexturePipeline(MetalDevice* device, std::vector<float> inputData, std::shared_ptr<Texture> outputTexture) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/buf2texture.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffer = {
      .data = std::move(inputData),
    },
    .threadSize = {width, height},
    .uniforms = {width, height},
    .textures = {outputTexture}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

std::shared_ptr<ComputePipeline> createTexture2TexturePipeline(MetalDevice* device, std::shared_ptr<Texture> inputTexture, std::shared_ptr<Texture> outputTexture) {
  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/texture2texture.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffer = {},
    .threadSize = {width, height},
    .uniforms = {},
    .textures = {inputTexture, outputTexture}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

int main() {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  auto device = std::make_unique<MetalDevice>();
  assert(device && "Failed to create device");

  auto commandQueue = device->createCommandQueue({});

  // Pipeline 0 input buffer

  std::vector<float> inputData(totalElements);
  for (size_t i = 0; i < totalElements; ++i)
    inputData[i] = static_cast<float>(i);

  // Pipeline 0

  auto pipeline0outputTexture = device->createTexture({
    .imageData = {
      .pixel = {},
      .width = width,
      .height = height,
      .channels = 1,
      .format = ImageFormat::Float32
    },
    .sampler = {},
    .usage = TextureUsage::WRITE | TextureUsage::READ,
    .storage = TextureStorage::SHARED,
    .loadType = TextureLoadType::EAGER,
    .pipeline = TexturePipeline::COMPUTE,
  });
  auto pipeline0_buf2tex = createBuf2TexturePipeline(device.get(), std::move(inputData), pipeline0outputTexture);

  // Pipeline 1 output texture

  auto pipeline1outputTexture = device->createTexture({
    .imageData = {
      .pixel = {},
      .width = width,
      .height = height,
      .channels = 1,
      .format = ImageFormat::Float32
    },
    .sampler = {},
    .usage = TextureUsage::WRITE,
    .storage = TextureStorage::SHARED,
    .loadType = TextureLoadType::EAGER,
    .pipeline = TexturePipeline::COMPUTE,
  });

  // Pipeline 1

  auto pipeline1_tex2tex = createTexture2TexturePipeline(device.get(), pipeline0outputTexture, pipeline1outputTexture);

  // Pipeline vector

  std::vector<std::shared_ptr<ComputePipeline>> pipelines = {pipeline0_buf2tex, pipeline1_tex2tex};

  // Encoding

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
  MTL::Region region = MTL::Region::Make2D(0, 0, width, height);
  SAFE_DOWN_CAST(MetalTexture*, pipeline1outputTexture.get())->getTextureHandle()->getBytes(outputBuffer.data(), width * sizeof(float), region, 0);

  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      size_t index = y * width + x;
      std::cout << outputBuffer[index] << "\t";
    }
    std::cout << std::endl;
  }

  return 0;
}
