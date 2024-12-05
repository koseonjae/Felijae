#include <Base/Utility/TypeCast.h>
#include <Graphics/Utility/MetalBlitCopy.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Metal/MetalCommandQueue.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace goala {
void blitTextureToDrawable(Texture* mlttexture, CommandQueue* commandQueue, void* drawable) {
  auto mtlDrawable = static_cast<CA::MetalDrawable*>(drawable);
  MTL::Texture* drawableTexture = mtlDrawable->texture();

  MTL::CommandBuffer* commandBuffer = SAFE_DOWN_CAST(MetalCommandQueue*, commandQueue)->getMTLCommandQueue()->commandBuffer();

  // encoding
  auto texture = SAFE_DOWN_CAST(MetalTexture*, mlttexture)->getTextureHandle();
  MTL::BlitCommandEncoder* blitEncoder = commandBuffer->blitCommandEncoder();
  assert(texture->width() == mtlDrawable->texture()->width() && "Incorrect texture width");
  assert(texture->height() == mtlDrawable->texture()->height() && "Incorrect texture height");
  blitEncoder->copyFromTexture(texture, drawableTexture);
  blitEncoder->endEncoding();

  // swap buffer
  commandBuffer->presentDrawable(mtlDrawable);

  commandBuffer->commit();
}
}
