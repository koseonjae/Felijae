#include <Base/Utility/FileReader.h>
#include <Base/Utility/ImageLoader.h>
#include <Base/Utility/ImageUtil.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Utility/MetalBlitCopy.h>
#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>
#include <Engine/Renderer/ForwardRenderer.h>

#include <SDL.h>

using namespace goala;

namespace {
const std::vector<int> viewport = {800, 600};
} // namespace

int main(int argc, char** argv) {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("SDL Metal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewport[0], viewport[1], SDL_WINDOW_METAL);

  int width, height;
  SDL_Metal_GetDrawableSize(window, &width, &height);
  assert(viewport[0] == width && viewport[1] == height && "Created window drawable size is not same with viewport");

  auto device = std::make_unique<MetalDevice>();

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setFramebufferOnly(false);
  layer->setDevice(device->getMTLDevice());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

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
      .enable = false,
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

  TextureDescription uniformTextureDesc = {
    .imageData = convertRGB2BGRA(ImageLoader::load(File("asset://model/suzanne/uvmap.jpeg"))),
    .sampler = {
      .minFilter = TextureFilter::LINEAR,
      .magFilter = TextureFilter::LINEAR,
      .mipFilter = TextureFilter::LINEAR,
      .wrapS = TextureWrap::CLAMP_TO_EDGE,
      .wrapT = TextureWrap::CLAMP_TO_EDGE,
    },
    .usage = TextureUsage::READ | TextureUsage::WRITE,
    .storage = TextureStorage::SHARED,
    .loadType = TextureLoadType::EAGER,
    .pipeline = TexturePipeline::FRAGMENT
  };
  auto uniformTexture = device->createTexture(uniformTextureDesc);

  // Uniforms
  auto uniforms = std::make_shared<Uniforms>();
  uniforms->setTexture("uTexture", uniformTexture);
  glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
  uniforms->setUniform("uEmitLight", emitLight);

  // Pipeline
  PipelineDescription pipelineDesc = {
    .shaders = {
      {
        .source = File("asset://shader/lighting.vert").read(),
        .type = ShaderType::VERTEX
      },
      {
        .source = File("asset://shader/lighting.frag").read(),
        .type = ShaderType::FRAGMENT
      }
    },
    .vertexBuffer = {
      .object = loadObj(File("asset://model/suzanne/suzanne.obj"))
    },
    .rasterizer = rasterizer,
    .outputMerger = outputMerger,
    .format = getImageFormat(layer->pixelFormat()),
    .uniforms = uniforms,
  };
  auto pipeline = device->createPipeline(pipelineDesc);

  // Queue
  CommandQueueDescription queueDesc{};
  auto queue = device->createCommandQueue(queueDesc);

  // Model
  auto model = std::make_shared<Model>();
  model->setPipeline(pipeline);

  // Camera
  glm::vec3 eye = glm::vec3(3.0, 3.0, 3.0);
  glm::vec3 at = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
  auto fovy = glm::radians<float>(90);
  auto aspectRatio = 1.f; // frustum width == height
  float n = 0.1f;
  float f = 100.0f;
  auto camera = std::make_shared<Camera>();
  camera->setCamera(eye, at, up);
  camera->setProjection(fovy, aspectRatio, n, f);

  // Light
  auto light = std::make_shared<Light>();
  light->setLightColor({1.0f, 1.0f, 1.0f});
  light->setLightDirection({0.0f, 0.0f, 1.0f});

  // Scene
  auto scene = std::make_shared<Scene>();
  scene->addModel(std::move(model));
  scene->setNode(std::move(camera));
  scene->setNode(std::move(light));

  // Renderer
  auto renderer = std::make_shared<ForwardRenderer>();
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

    renderer->update();

    CommandBufferDescription commandBufferDesc{};
    auto cmdBuf = queue->createCommandBuffer(commandBufferDesc);
    renderer->render(cmdBuf);

    // Draw offscreen texture to window swap chain
    auto drawable = layer->nextDrawable();
    auto metalTexture = std::static_pointer_cast<MetalTexture>(texture);
    blitTextureToDrawable(metalTexture->getTextureHandle(), drawable, std::static_pointer_cast<MetalCommandQueue>(queue)->getMTLCommandQueue());
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
