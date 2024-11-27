#include <Base/Object/Object.h>
#include <Base/Object/Triangle.h>
#include <Base/Utility/FileReader.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalOutputMerger.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRasterizer.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Utility/MetalBlitCopy.h>
#include <Graphics/Utility/MetalRef.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>
#include <Engine/Renderer/ForwardRenderer.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <SDL.h>

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

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("SDL Metal", -1, -1, viewport[0], viewport[1], SDL_WINDOW_METAL);

  int width, height;
  SDL_Metal_GetDrawableSize(window, &width, &height);
  assert(viewport[0] == width && viewport[1] == height && "Created window drawable size is not same with viewport");

  auto device = std::make_unique<MetalDevice>(MTL::CreateSystemDefaultDevice());

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setDevice(device->getMTLDevice());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

  // Renderer
  auto renderer = std::make_shared<ForwardRenderer>();

  // Shader
  auto vertexFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_triangle.vert").getPath()), ShaderType::VERTEX);
  auto fragmentFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_triangle.frag").getPath()), ShaderType::FRAGMENT);
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
  auto outputMerger = std::make_shared<MetalOutputMerger>(device.get());
  outputMerger->setDepthTest(depthTest);
  outputMerger->setAlphaBlend(alphaBlend);
  metalPipelineDesc.outputMerger = outputMerger;

  // Buffer
  BufferDescription bufferDesc = {
    .object = Triangle::load()
  };
  auto vertexBuffer = device->createBuffer(bufferDesc);
  metalPipelineDesc.buffer = vertexBuffer;
  metalPipelineDesc.format = getImageFormat(layer->pixelFormat());
  auto metalPipeline = device->createPipeline(metalPipelineDesc);

  // RenderPass
  auto renderPass = std::make_shared<MetalRenderPass>();
  renderer->setRenderPass(renderPass);

  // Queue
  auto queue = makeMetalRef(device->getMTLDevice()->newCommandQueue());

  // Model
  auto model = std::make_shared<Model>();
  model->setPipeline(metalPipeline);

  // Scene
  auto scene = std::make_shared<Scene>();
  scene->addModel(std::move(model));
  renderer->setScene(std::move(scene));

  // Texture
  auto texture = std::make_shared<MetalTexture>(device.get());
  texture->initialize(width, height, getImageFormat(MTL::PixelFormatBGRA8Unorm), false);

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

    auto drawable = layer->nextDrawable();

    std::vector<Attachment> attachments;
    attachments.emplace_back(Attachment{
      .type = AttachmentType::Color,
      .loadFunc = LoadFunc::Clear,
      .storeFunc = StoreFunc::Store,
      .clear = ClearColor{0.0f, 0.0f, 1.0f, 1.0f},
      .texture = texture,
    });
    renderPass->setAttachments(std::move(attachments));

    auto cmdBuf = std::make_shared<MetalCommandBuffer>(device.get(), queue->commandBuffer());
    auto blitCmdBuf = std::make_shared<MetalCommandBuffer>(device.get(), queue->commandBuffer());
    blitCmdBuf->addDependency(cmdBuf.get());

    renderer->update();
    renderer->render(cmdBuf);

    // Draw offscreen texture to window swap chain
    blitTextureToDrawable(texture->getHandle(), drawable, queue.get());
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
