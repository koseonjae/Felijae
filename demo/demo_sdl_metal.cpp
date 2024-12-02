#include <Base/Object/Object.h>
#include <Base/Object/Polygons.h>
#include <Base/Utility/FileReader.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalPipeline.h>
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
  ShaderDescription vertShaderDesc = {
    .source = readFile(File("asset://shader/metal_color.vert").getPath()),
    .type = ShaderType::VERTEX
  };
  ShaderDescription fragShaderDesc = {
    .source = readFile(File("asset://shader/metal_color.frag").getPath()),
    .type = ShaderType::FRAGMENT
  };
  auto vertexFunc = device->createShader(vertShaderDesc);
  auto fragmentFunc = device->createShader(fragShaderDesc);

  // Rasterizer
  Rasterizer rasterizer = {
    .culling = {
      .enable = true,
      .frontFace = Culling::FrontFace::CCW,
      .cullMode = Culling::CullMode::Back,
    },
    .viewport = {
      .minX = 0,
      .minY = 0,
      .width = viewport[0],
      .height = viewport[1],
      .minZ = 0.0f,
      .maxZ = 1.0f,
    }
  };

  // OutputMerger
  OutputMerger outputMerger = {
    .depthTest = {
      .enable = true,
      .depthFunc = DepthTest::DepthTestFunc::Less,
      .updateDepthMask = true,
    },
    .alphaBlend = {
      .enable = true,
      .fragmentBlendFunc = AlphaBlend::BlendFunc::SRC_ALPHA,
      .pixelBlendFunc = AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA,
      .blendEquation = AlphaBlend::BlendEquation::Add,
    }
  };

  // Buffer
  BufferDescription bufferDesc = {
    .object = Polygons::triangle()
  };
  auto vertexBuffer = device->createBuffer(bufferDesc);

  PipelineDescription metalPipelineDesc{
    .shaders = {std::move(vertexFunc), std::move(fragmentFunc)},
    .buffer = std::move(vertexBuffer),
    .rasterizer = rasterizer,
    .outputMerger = outputMerger,
    .format = getImageFormat(layer->pixelFormat()),
  };
  auto metalPipeline = device->createPipeline(metalPipelineDesc);

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

  // RenderPass
  RenderPassDescription renderPassDesc = {
    .attachments = {
      {
        .type = AttachmentType::Color,
        .loadFunc = LoadFunc::Clear,
        .storeFunc = StoreFunc::Store,
        .clear = ClearColor{0.0f, 0.0f, 1.0f, 1.0f},
        .texture = texture,
      }
    }
  };
  auto renderPass = device->createRenderPass(std::move(renderPassDesc));
  renderer->setRenderPass(renderPass);

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
