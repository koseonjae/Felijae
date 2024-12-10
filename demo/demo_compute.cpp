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
    .uniforms = {width, height},
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
    .uniforms = {width, height},
    .textures = {inputTexture, outputTexture}
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
    .uniforms = {width, height},
    .textures = {inputTexture}
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
  auto pipeline0_inputBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(inputData.data(), inputData.size() * sizeof(float), MTL::ResourceStorageModeShared));

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
  auto pipeline0_buf2tex = createBuf2TexturePipeline(device.get(), pipeline0_inputBuffer, pipeline0outputTexture);

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

  // Pipeline 2 output buffer

  std::vector<float> outputData(totalElements);
  auto pipeline2_outputBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(outputData.data(), outputData.size() * sizeof(float), MTL::ResourceStorageModeShared));
  auto pipeline2_tex2buf = createTexture2BufferPipeline(device.get(), pipeline1outputTexture, pipeline2_outputBuffer);

  // Pipeline vector

  std::vector<std::shared_ptr<ComputePipeline>> pipelines = {pipeline0_buf2tex, pipeline1_tex2tex, pipeline2_tex2buf};

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

  // texture to array
  // MTL::Region region = MTL::Region::Make2D(0, 0, width, height);
  // SAFE_DOWN_CAST(MetalTexture*, pipeline1outputTexture.get())->getTextureHandle()->getBytes(outputBuffer.data(), width * sizeof(float), region, 0);

  // buffer to array
  std::memcpy(outputBuffer.data(), pipeline2_outputBuffer->contents(), sizeof(float) * totalElements);

  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      size_t index = y * width + x;
      std::cout << outputBuffer[index] << "\t";
    }
    std::cout << std::endl;
  }

  return 0;
}
