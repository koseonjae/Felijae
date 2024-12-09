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

int main() {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  auto device = std::make_unique<MetalDevice>();
  assert(device && "Failed to create device");

  auto commandQueue = device->createCommandQueue({});

  // 행렬 크기 정의
  constexpr int width = 4;
  constexpr int height = 10;
  constexpr int totalElements = width * height;

  std::vector<float> inputData(totalElements);
  for (size_t i = 0; i < totalElements; ++i)
    inputData[i] = static_cast<float>(i);

  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/compute_example.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffer = {
      .data = std::move(inputData),
      .width = width,
      .height = height,
    },
  };

  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  auto mtlPipeline = SAFE_DOWN_CAST(MetalComputePipeline*, pipeline.get());

  // 출력 텍스처 생성
  auto textureDescriptor = MTL::TextureDescriptor::texture2DDescriptor(
    MTL::PixelFormatRGBA32Float, width, height, false);
  textureDescriptor->setUsage(MTL::TextureUsageShaderWrite);

  TextureDescription textureDesc = {
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
  };
  auto outputTexture = device->createTexture(textureDesc);

  // 커맨드 버퍼와 인코더 생성
  auto commandBuffer = commandQueue->createCommandBuffer({});
  auto mtlCmdBuffer = SAFE_DOWN_CAST(MetalCommandBuffer*, commandBuffer.get());
  auto computeEncoder = mtlCmdBuffer->getCommandBuffer()->computeCommandEncoder();

  mtlPipeline->encode(computeEncoder, outputTexture.get());

  commandBuffer->commit();
  commandBuffer->waitUntilCompleted();

  // 결과 확인
  std::array<float, totalElements> outputBuffer{};
  MTL::Region region = MTL::Region::Make2D(0, 0, width, height);
  SAFE_DOWN_CAST(MetalTexture*, outputTexture.get())->getTextureHandle()->getBytes(outputBuffer.data(), width * sizeof(float), region, 0);

  std::cout << "결과 텍스처 값:" << std::endl;
  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      size_t index = y * width + x;
      std::cout << outputBuffer[index] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
