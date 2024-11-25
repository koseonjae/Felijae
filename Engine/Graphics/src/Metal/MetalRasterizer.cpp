#include <Graphics/Metal/MetalRasterizer.h>
#include <Graphics/Metal/MetalCommandEncoder.h>

#include <Metal/Metal.hpp>

using namespace goala;

void MetalRasterizer::bind(void* descriptor) {
  auto metalDescriptor = static_cast<MTL::RenderPipelineDescriptor*>(descriptor);

  metalDescriptor->setRasterizationEnabled(true);

  _updateCulling(metalDescriptor);
  _updateViewPort(metalDescriptor);
}

void MetalRasterizer::encode(CommandEncoder* encoder) {
  auto metalEncoder = dynamic_cast<MetalCommandEncoder*>(encoder);
}

void MetalRasterizer::_updateCulling(MTL::RenderPipelineDescriptor* descriptor) {
  auto& culling = getCulling();
}

void MetalRasterizer::_updateViewPort(MTL::RenderPipelineDescriptor* descriptor) {

}
