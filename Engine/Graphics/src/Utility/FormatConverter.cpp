#include <Base/File/File.h>
#include <Base/ImageData.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Utility/FormatConverter.h>
#include <Graphics/Utility/MetalRef.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalComputePipeline.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Model/ComputePipeline.h>

#include <Metal/Metal.hpp>

using namespace goala;

void FormatConverter::rgb2bgra(MetalDevice* device, const ImageData& image, MTL::Texture* targetTexture) {
  if (image.pixel.empty())
    assert(false && "pixel is empty");
  assert(image.pixelFormat == ImageFormat::RGB && "Inconsistent pixel format");

  auto inputBuffer = makeMetalRef(device->getMTLDevice()->newBuffer(image.pixel.data(), image.pixel.size() * sizeof(uint8_t), MTL::ResourceStorageModeShared));

  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/rgb_buf_to_bgra_texture.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {inputBuffer},
    .threadSize = {image.width, image.height},
    .textures = {targetTexture}
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));

  auto commandQueue = device->createCommandQueue({});
  auto commandBuffer = commandQueue->createCommandBuffer({});
  auto mtlCmdBuffer = SAFE_DOWN_CAST(MetalCommandBuffer*, commandBuffer.get());

  auto computeEncoder = mtlCmdBuffer->getCommandBuffer()->computeCommandEncoder();
  SAFE_DOWN_CAST(MetalComputePipeline*, pipeline.get())->encode(computeEncoder);

  commandBuffer->commit();
  commandBuffer->waitUntilCompleted();
}
