#include <Graphics/Utility/MetalBlitCopy.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace goala {

void blitTextureToDrawable(MTL::Texture* texture, CA::MetalDrawable* drawable, MTL::CommandQueue* commandQueue) {
  MTL::Texture* drawableTexture = drawable->texture();
  MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();

  // encoding
  MTL::BlitCommandEncoder* blitEncoder = commandBuffer->blitCommandEncoder();
  assert(texture->width() == drawable->texture()->width() && "Incorrect texture width");
  assert(texture->height() == drawable->texture()->height() && "Incorrect texture height");
  blitEncoder->copyFromTexture(texture, drawableTexture);
  blitEncoder->endEncoding();

  // swap buffer
  commandBuffer->presentDrawable(drawable);

  commandBuffer->commit();

  commandBuffer->waitUntilCompleted();
}

}