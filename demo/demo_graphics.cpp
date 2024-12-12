#include <Base/Utility/FileReader.h>
#include <Base/Utility/ImageLoader.h>
#include <Base/Utility/ImageUtil.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Utility/MetalBlitCopy.h>
#include <Graphics/Utility/OpenGLBlitCopy.h>
#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/ComputePipeline.h>
#include <Graphics/Model/CommandQueue.h>
#include <Graphics/Model/Texture.h>
#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalComputePipeline.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>
#include <Engine/Renderer/ForwardRenderer.h>
#include <SDLWrapper/MetalSDLWrapper.h>
#include <SDLWrapper/OpenGLSDLWrapper.h>

#include <Metal/Metal.hpp>

using namespace goala;

namespace {
ImageFormat getImageFormatSDLFormat(SDL_PixelFormatEnum pixelFormat) {
  switch (pixelFormat) {
    case SDL_PIXELFORMAT_RGBA8888: return ImageFormat::RGBA;
    case SDL_PIXELFORMAT_BGRA8888: return ImageFormat::BGRA;
    default:
      assert(false && "Format not supported");
  }
}

Graphics parseGraphicsOption(const std::string& arg) {
  if (arg == "OpenGL")
    return Graphics::OpenGL3;
  if (arg == "Metal")
    return Graphics::Metal;
  assert(false && "Invalid graphics option");
}
} // namespace

std::shared_ptr<ComputePipeline> createGrayscalePipeline(Device* device, std::shared_ptr<Texture>& texture) {
  int width = texture->getDescription().imageData.width;
  int height = texture->getDescription().imageData.height;

  ComputePipelineDescription pipelineDesc = {
    .shader = {
      .source = File("asset://shader/compute_grayscale.msl").read(),
      .type = ShaderType::COMPUTE
    },
    .buffers = {},
    .threadSize = {width, height},
    .textures = {texture},
  };
  auto pipeline = device->createComputePipeline(std::move(pipelineDesc));
  return pipeline;
}

std::shared_ptr<Model> createSuzaneModel(Device* device, std::shared_ptr<RenderPass> renderPass) {
  auto& colorAttachmentDesc = renderPass->getDescription().attachments.at(0).texture->getDescription();
  auto width = colorAttachmentDesc.imageData.width;
  auto height = colorAttachmentDesc.imageData.height;
  auto format = colorAttachmentDesc.textureFormat;

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
      .width = width,
      .height = height,
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
    .imageData = ImageLoader::load(File("asset://model/suzanne/uvmap.jpeg")),
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
    .pipeline = TexturePipeline::FRAGMENT,
    .textureFormat = format
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
    .format = format,
    .uniforms = uniforms,
  };
  auto pipeline = device->createPipeline(pipelineDesc);

  // Model
  auto model = std::make_shared<Model>();
  model->setPipeline(pipeline);

  return model;
}

std::shared_ptr<Model> createAxisModel(Device* device, std::shared_ptr<RenderPass> renderPass) {
  auto& colorAttachmentDesc = renderPass->getDescription().attachments.at(0).texture->getDescription();
  auto width = colorAttachmentDesc.imageData.width;
  auto height = colorAttachmentDesc.imageData.height;
  auto format = colorAttachmentDesc.textureFormat;

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
      .width = width,
      .height = height,
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

  // Uniforms
  auto uniforms = std::make_shared<Uniforms>();
  glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
  uniforms->setUniform("uEmitLight", emitLight);

  // Pipeline
  PipelineDescription pipelineDesc = {
    .shaders = {
      {
        .source = File("asset://shader/axis.vert").read(),
        .type = ShaderType::VERTEX
      },
      {
        .source = File("asset://shader/axis.frag").read(),
        .type = ShaderType::FRAGMENT
      }
    },
    .vertexBuffer = {
      .object = loadObj(File("asset://model/axis/axis.obj"))
    },
    .rasterizer = rasterizer,
    .outputMerger = outputMerger,
    .format = format,
    .uniforms = uniforms,
  };
  auto pipeline = device->createPipeline(pipelineDesc);

  // Model
  auto model = std::make_shared<Model>();
  model->setPipeline(pipeline);

  return model;
}

int main(int argc, char** argv) {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  Graphics graphics = argc > 1 ? parseGraphicsOption(argv[1]) : Graphics::Metal;

  int width = 800;
  int height = 600;
  std::unique_ptr<Device> device;
  std::unique_ptr<SDLWrapper> sdl;
  if (graphics == Graphics::OpenGL3) {
    device = std::make_unique<OpenGLDevice>();
    sdl = std::make_unique<OpenGLSDLWrapper>(width, height);
  }
  else {
    device = std::make_unique<MetalDevice>();
    sdl = std::make_unique<MetalSDLWrapper>(width, height, SAFE_DOWN_CAST(MetalDevice*, device.get())->getMTLDevice());
  }

  auto drawableSize = sdl->getDrawableSize();
  assert(width == std::get<0>(drawableSize) && height == std::get<1>(drawableSize) && "Wrong drawable size");

  // Offscreen Texture
  auto renderTargetTexture = device->createTexture({
    .imageData = {
      .width = width,
      .height = height,
      .pixel = {},
      .pixelFormat = {}
    },
    .loadType = TextureLoadType::EAGER,
    .textureFormat = getImageFormatSDLFormat(sdl->getPixelFormat()),
  });

  // suzane renderer 1
  auto suzaneRenderPass = device->createRenderPass({
    .attachments = {
      {
        .type = AttachmentType::Color,
        .loadFunc = LoadFunc::Clear,
        .storeFunc = StoreFunc::Store,
        .clear = ClearColor{0.0f, 0.5f, 0.5f, 1.0f},
        .texture = renderTargetTexture,
      }
    }
  });

  auto axisRenderPass = device->createRenderPass({
    .attachments = {
      {
        .type = AttachmentType::Color,
        .loadFunc = LoadFunc::Load,
        .storeFunc = StoreFunc::Store,
        .clear = {},
        .texture = renderTargetTexture,
      }
    }
  });

  auto suzaneModel1 = createSuzaneModel(device.get(), suzaneRenderPass);
  auto suzaneModel2 = createSuzaneModel(device.get(), suzaneRenderPass);
  suzaneModel2->translate({3.0f, 0.0f, 0.0f});

  // gray scale compute pipeline
  auto grayscalePipeline = graphics == Graphics::Metal ? createGrayscalePipeline(device.get(), renderTargetTexture) : nullptr;

  auto axisModel = createAxisModel(device.get(), axisRenderPass);
  axisModel->scale({0.1f, 0.1f, 0.1f});

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

  // Suzane
  auto suzaneScene = std::make_shared<Scene>();
  suzaneScene->addModel(suzaneModel1);
  suzaneScene->addModel(suzaneModel2);
  suzaneScene->setNode(camera);
  suzaneScene->setNode(light);
  auto suzaneRenderer = std::make_shared<ForwardRenderer>();
  suzaneRenderer->setScene(std::move(suzaneScene));
  suzaneRenderer->setRenderPass(std::move(suzaneRenderPass));

  // Axis
  auto axisScene = std::make_shared<Scene>();
  axisScene->addModel(axisModel);
  axisScene->setNode(camera);
  axisScene->setNode(light);
  auto axisRenderer = std::make_shared<ForwardRenderer>();
  axisRenderer->setScene(std::move(axisScene));
  axisRenderer->setRenderPass(std::move(axisRenderPass));

  // Queue
  CommandQueueDescription queueDesc{};
  auto queue = device->createCommandQueue(queueDesc);

  sdl->setUpdateCallback([&]() {
    suzaneRenderer->update();
    axisRenderer->update();
  });

  sdl->setRenderCallback([&]() {
    auto cmdBuf = queue->createCommandBuffer({});
    suzaneRenderer->render(cmdBuf);
    if (grayscalePipeline) { // metal only
      auto encoder = SAFE_DOWN_CAST(MetalCommandBuffer*, cmdBuf.get())->getCommandBuffer()->computeCommandEncoder();
      SAFE_DOWN_CAST(MetalComputePipeline*, grayscalePipeline.get())->encode(encoder);
    }
    axisRenderer->render(cmdBuf);
    cmdBuf->commit();
  });

  sdl->setBlitCopyCallback([&](void* drawable) {
    if (graphics == Graphics::OpenGL3)
      blitCopyFrameBufferToScreen(axisRenderer->getRenderPass(), width, height);
    else
      blitTextureToDrawable(renderTargetTexture.get(), queue.get(), drawable);
  });

  sdl->loop();

  return 0;
}
