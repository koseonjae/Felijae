#include <Base/Object/Object.h>
#include <Base/Object/Polygons.h>
#include <Base/Utility/FileReader.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalOutputMerger.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRasterizer.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Utility/MetalBlitCopy.h>
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
  SDL_Window* window = SDL_CreateWindow("SDL Metal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewport[0], viewport[1], SDL_WINDOW_METAL);

  int width, height;
  SDL_Metal_GetDrawableSize(window, &width, &height);
  assert(viewport[0] == width && viewport[1] == height && "Created window drawable size is not same with viewport");

  auto device = std::make_unique<MetalDevice>();

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setDevice(device->getMTLDevice());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

  // Renderer
  auto renderer = std::make_shared<ForwardRenderer>();

  // Shader
  auto vertexFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_color.vert").getPath()), ShaderType::VERTEX);
  auto fragmentFunc = std::make_shared<MetalShader>(device.get(), readFile(File("asset://shader/metal_color.frag").getPath()), ShaderType::FRAGMENT);
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
    .object = Polygons::triangle()
  };
  auto vertexBuffer = device->createBuffer(bufferDesc);
  metalPipelineDesc.buffer = vertexBuffer;
  metalPipelineDesc.format = getImageFormat(layer->pixelFormat());
  auto metalPipeline = device->createPipeline(metalPipelineDesc);

  // RenderPass
  auto renderPass = std::make_shared<MetalRenderPass>();
  renderer->setRenderPass(renderPass);

  // Queue
  CommandQueueDescription queueDesc{};
  auto queue = device->createCommandQueue(queueDesc);

  // Model
  auto model = std::make_shared<Model>();
  model->setPipeline(metalPipeline);

  // Scene
  auto scene = std::make_shared<Scene>();
  scene->addModel(std::move(model));
  renderer->setScene(std::move(scene));

  // Texture
  TextureDescription textureDesc = {
    .imageData = {
      .width = width,
      .height = height,
      .pixel = {},
      .format = getImageFormat(MTL::PixelFormatBGRA8Unorm),
    },
    .loadType = TextureLoadType::EAGER,
  };
  auto texture = device->createTexture(textureDesc);

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

    CommandBufferDescription commandBufferDesc{};
    auto cmdBuf = queue->createCommandBuffer(commandBufferDesc);
    auto blitCmdBuf = queue->createCommandBuffer(commandBufferDesc);
    // blitCmdBuf->addDependency(cmdBuf.get()); // todo: fence 임시로 없앴음

    renderer->update();
    renderer->render(cmdBuf);

    // Draw offscreen texture to window swap chain
    auto metalTexture = std::static_pointer_cast<MetalTexture>(texture);
    blitTextureToDrawable(metalTexture->getHandle(), drawable, std::static_pointer_cast<MetalCommandQueue>(queue)->getMTLCommandQueue());
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
