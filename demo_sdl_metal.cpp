#include <Graphics/Utility/FileReader.h>
#include <Graphics/Utility/MetalRef.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Model/Object.h>
#include <Graphics/Model/Triangle.h>

#include <Foundation/Foundation.hpp>
#include <Metal/triangle_types.h>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <SDL.h>

#include <iostream>

namespace {
const std::vector<int> viewport = {640, 480};

inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

int main(int argc, char** argv) {
  NS::Error* err = nil;

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("SDL Metal", -1, -1, viewport[0], viewport[1], SDL_WINDOW_ALLOW_HIGHDPI);

  MetalDevice device(MTL::CreateSystemDefaultDevice());

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setDevice(device.get());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

  Object obj = Triangle::load();
  MetalBuffer vertexBuffer(&device, obj);

  MetalShader vertexFunc(&device, readFile("../asset/shader/metal_triangle.vert"), ShaderType::VERTEX);
  MetalShader fragmentFunc(&device, readFile("../asset/shader/metal_triangle.frag"), ShaderType::FRAGMENT);

  auto pipelineDesc = MetalRef(MTL::RenderPipelineDescriptor::alloc()->init());
  pipelineDesc->setVertexFunction(vertexFunc.get());
  pipelineDesc->setFragmentFunction(fragmentFunc.get());
  pipelineDesc->setVertexDescriptor(vertexBuffer.getVertexDescriptor());

  auto colorAttachmentDesc = pipelineDesc->colorAttachments()->object(0);
  colorAttachmentDesc->setPixelFormat(layer->pixelFormat());

  auto pipeline = MetalRef(device.get()->newRenderPipelineState(pipelineDesc.get(), &err));
  assert(pipeline && "Failed to create pipeline");

  auto queue = MetalRef(device.get()->newCommandQueue());

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }
      }
    }

    auto drawable = MetalRef(layer->nextDrawable());

    auto pass = MetalRef(MTL::RenderPassDescriptor::renderPassDescriptor());

    auto colorAttachment = pass->colorAttachments()->object(0);
    colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
    colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    colorAttachment->setClearColor(MTL::ClearColor(0.0, 0.0, 1.0, 1.0));
    colorAttachment->setTexture(drawable->texture());

    auto buffer = MetalRef(queue->commandBuffer());

    // encoding
    auto encoder = MetalRef(buffer->renderCommandEncoder(pass.get()));
    encoder->setRenderPipelineState(pipeline.get());
    encoder->setVertexBuffer(vertexBuffer.getVertexHandle(), 0, AAPLVertexInputIndexVertices);
    encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                   obj.indices.size(),
                                   MTL::IndexTypeUInt32,
                                   vertexBuffer.getIndexHandle(),
                                   0);
    encoder->endEncoding();

    buffer->presentDrawable(drawable.get());
    buffer->commit();
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}