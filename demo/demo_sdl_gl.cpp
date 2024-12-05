#include <Base/Utility/ImageLoader.h>
#include <Base/Utility/TypeCast.h>
#include <Base/File/File.h>
#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Model/CommandQueue.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <Engine/Model/Light.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>
#include <Engine/Renderer/ForwardRenderer.h>

#include "SDLWrapper/OpenGLSDLWrapper.h"

#include <glm/glm.hpp>

using namespace goala;

int main() {
  File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  OpenGLSDLWrapper sdl(Graphics::OpenGL3, 800, 600);

  auto [width, height] = sdl.getDrawableSize();

  auto device = std::make_unique<OpenGLDevice>();

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

  // Output Merger
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

  // Uniform
  auto uniforms = std::make_shared<Uniforms>();
  glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
  uniforms->setUniform("uEmitLight", emitLight);

  TextureDescription textureDesc = {
    .imageData = ImageLoader::load(File("asset://model/suzanne/uvmap.jpeg")),
    .loadType = TextureLoadType::EAGER,
  };
  auto texture = device->createTexture(textureDesc);
  uniforms->setTexture("uTexture", texture);

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
    .uniforms = uniforms,
    .format = ImageFormat::RGBA,
  };
  auto pipeline = device->createPipeline(pipelineDesc);

  auto renderer = std::make_shared<ForwardRenderer>();

  // Render Pass
  {
    TextureDescription colorTextureDesc = {
      .imageData = {
        .width = width,
        .height = height,
        .format = ImageFormat::RGBA,
        .pixel = {},
      },
      .loadType = TextureLoadType::EAGER,
    };
    auto colorTexture = device->createTexture(colorTextureDesc);

    RenderPassDescription renderPassDesc = {
      .attachments = {
        {
          .type = AttachmentType::Color,
          .loadFunc = LoadFunc::Clear,
          .storeFunc = StoreFunc::Store,
          .clear = ClearColor{1.0f, 1.0f, 0.0f, 1.0f},
          .texture = std::move(colorTexture),
        }
      }
    };
    auto renderPass = device->createRenderPass(std::move(renderPassDesc));
    renderer->setRenderPass(renderPass);
  }

  // Scene
  auto scene = std::make_shared<Scene>();
  renderer->setScene(scene);

  // Light
  {
    auto light = std::make_shared<Light>();
    light->setLightColor({1.0f, 1.0f, 1.0f});
    light->setLightDirection({0.0f, 0.0f, 1.0f});
    scene->setNode(std::move(light));
  }

  // Camera
  {
    // note: 0이면 투영행렬 정의상 분모가 0이되어 미정의 동작,
    // 0.01, 0.001등 너무 작으면 정밀도가 낮아져 가까운 물체들에 대해 z fighting 발생할 수 있음
    // => depth bit 조절 필요
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
    scene->setNode(std::move(camera));
  }

  // Model
  {
    auto model = std::make_shared<Model>();
    model->setPipeline(pipeline);
    scene->addModel(std::move(model));
  }

  CommandQueueDescription queueDesc{};
  auto cmdQueue = device->createCommandQueue(queueDesc);

  sdl.setUpdateCallback([&]() {
    renderer->update();
  });

  sdl.setRenderCallback([&]() {
    CommandBufferDescription cmdBufDesc{};
    auto cmdBuf = cmdQueue->createCommandBuffer(cmdBufDesc);
    renderer->render(cmdBuf);
  });

  sdl.setBlitCopyCallback([&](void*) {
    auto openGLRenderPass = SAFE_DOWN_CAST(OpenGLRenderPass*, renderer->getRenderPass());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLRenderPass->getFrameBuffer(0).getHandle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
      0, 0, width, height, // 소스 영역 (FBO)
      0, 0, width, height, // 대상 영역 (화면)
      GL_COLOR_BUFFER_BIT, GL_NEAREST);
  });

  sdl.loop();

  return 0;
}
