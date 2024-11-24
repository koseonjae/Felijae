#include <Base/Object/Object.h>
#include <Base/Object/Triangle.h>
#include <Base/Utility/FileReader.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalOutputMerger.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRasterizer.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Utility/MetalRef.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <Metal/triangle_types.h>
#include <QuartzCore/QuartzCore.hpp>
#include <SDL.h>

#include <iostream>
#include <memory>

using namespace goala;

namespace {
const std::vector<int> viewport = {640, 480};

inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

int main(int argc, char** argv) {
  File::registerPath("../../demo/asset", "asset://");

  NS::Error* err = nil;

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("SDL Metal", -1, -1, viewport[0], viewport[1], SDL_WINDOW_ALLOW_HIGHDPI);

  auto device = std::make_unique<MetalDevice>(MTL::CreateSystemDefaultDevice());

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setDevice(device->getMTLDevice());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

  // Shader
  auto vertexFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_triangle.vert").getPath()), ShaderType::VERTEX);
  auto fragmentFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_triangle.frag").getPath()), ShaderType::FRAGMENT);
  auto metalPipeline = std::make_shared<MetalPipeline>(device.get());
  PipelineDescription metalPipelineDesc{};
  metalPipelineDesc.shaders.push_back(vertexFunc);
  metalPipelineDesc.shaders.push_back(fragmentFunc);

  // Rasterizer
  Culling culling = {
    .enable = true,
    .frontFace = Culling::FrontFace::CCW,
    .cullMode = Culling::CullMode::Back,
  };
  Viewport pipelineViewport = {
    .minX = 0,
    .minY = 0,
    .width = viewport[0],
    .height = viewport[1],
    .minZ = 0.0f,
    .maxZ = 1.0f,
  };
  auto rasterizer = std::make_shared<MetalRasterizer>();
  rasterizer->setCulling(culling);
  rasterizer->setViewport(pipelineViewport);
  metalPipelineDesc.rasterizer = rasterizer;

  // OutputMerger
  DepthTest depthTest = {
    .enable = true,
    .depthFunc = DepthTest::DepthTestFunc::Less,
    .updateDepthMask = true,
  };
  AlphaBlend alphaBlend = {
    .enable = true,
    .fragmentBlendFunc = AlphaBlend::BlendFunc::SRC_ALPHA,
    .pixelBlendFunc = AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA,
    .blendEquation = AlphaBlend::BlendEquation::Add,
  };
  auto outputMerger = std::make_shared<MetalOutputMerger>();
  outputMerger->setDepthTest(depthTest);
  outputMerger->setAlphaBlend(alphaBlend);
  metalPipelineDesc.outputMerger = outputMerger;

  // Buffer
  Object obj = Triangle::load();
  auto vertexBuffer = std::make_shared<MetalBuffer>(device.get(), obj);
  metalPipelineDesc.buffer = vertexBuffer;

  metalPipelineDesc.format = getImageFormat(layer->pixelFormat());

  metalPipeline->initialize(metalPipelineDesc);

  auto queue = MetalRef(device->getMTLDevice()->newCommandQueue());

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

    auto cmdBuf = MetalRef(queue->commandBuffer());

    // encoding
    auto encoder = MetalRef(cmdBuf->renderCommandEncoder(pass.get()));
    encoder->setRenderPipelineState(metalPipeline->getPipeline());
    encoder->setVertexBuffer(vertexBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
    encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                   obj.indices.size(),
                                   MTL::IndexTypeUInt32,
                                   vertexBuffer->getIndexHandle(),
                                   0);
    encoder->endEncoding();

    cmdBuf->presentDrawable(drawable.get());
    cmdBuf->commit();
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
