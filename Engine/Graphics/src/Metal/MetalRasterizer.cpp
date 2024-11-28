#include <Graphics/Metal/MetalRasterizer.h>
#include <Graphics/Metal/MetalCommandEncoder.h>

#include <Metal/Metal.hpp>

using namespace goala;

void MetalRasterizer::bind(void* descriptor) {}

void MetalRasterizer::encode(CommandEncoder* encoder) {
  auto metalEncoder = static_cast<MetalCommandEncoder*>(encoder);
  auto mtlEncoder = metalEncoder->getEncoder();
  _updateCulling(mtlEncoder);
  _updateViewPort(mtlEncoder);
}

void MetalRasterizer::_updateCulling(MTL::RenderCommandEncoder* encoder) {
  auto& culling = getCulling();

  if (!culling.enable) {
    encoder->setCullMode(MTL::CullModeNone);
    return;
  }

  if (culling.cullMode == Culling::CullMode::Front)
    encoder->setCullMode(MTL::CullModeFront);
  else
    encoder->setCullMode(MTL::CullModeBack);

  if (culling.frontFace == Culling::FrontFace::CCW)
    encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
  else
    encoder->setFrontFacingWinding(MTL::WindingClockwise);
}

void MetalRasterizer::_updateViewPort(MTL::RenderCommandEncoder* encoder) {
  auto viewport = getViewport();
  MTL::Viewport mltViewport = {
    .originX = static_cast<double>(viewport.minX),
    .originY = static_cast<double>(viewport.minY),
    .width = static_cast<double>(viewport.width),
    .height = static_cast<double>(viewport.height),
    .znear = static_cast<double>(viewport.minZ),
    .zfar = static_cast<double>(viewport.maxZ),
  };
  encoder->setViewport(mltViewport);
}
