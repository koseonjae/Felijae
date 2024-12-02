#include <Base/File/File.h>
#include <Base/Object/Polygons.h>
#include <Base/Utility/ImageLoader.h>
#include <Base/Utility/TypeCast.h>
#include <Engine/Model/Light.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>
#include <Engine/Renderer/ForwardRenderer.h>
#include <Graphics/OpenGL/OpenGLCommandBuffer.h>
#include <Graphics/OpenGL/OpenGLCommandQueue.h>
#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/OpenGL/OpenGLOutputMerger.h>
#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLProgram.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

using namespace goala;

int main() {
  File::registerPath("../../demo/asset", "asset://");

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow("SDL OpenGL",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        800,
                                        600,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  SDL_AddEventWatch([](void* data, SDL_Event* event) {
    switch (event->window.event) {
      case SDL_WINDOWEVENT_RESIZED:
      case SDL_WINDOWEVENT_SIZE_CHANGED: {
        auto width = event->window.data1;
        auto height = event->window.data2;
        // todo: impl
      }
      default: {
        break;
      }
    }
    return 0;
  }, window);

  int width, height;
  SDL_GL_GetDrawableSize(window, &width, &height);

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
  DepthTest depthTest = {
    .enable = true,
    .depthFunc = DepthTest::DepthTestFunc::Less,
    .updateDepthMask = true
  };
  AlphaBlend alphaBlend = {
    .enable = true,
    .fragmentBlendFunc = AlphaBlend::BlendFunc::SRC_ALPHA,
    .pixelBlendFunc = AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA,
    .blendEquation = AlphaBlend::BlendEquation::Add
  };
  auto outputMerger = std::make_shared<OpenGLOutputMerger>();
  outputMerger->setDepthTest(depthTest);
  outputMerger->setAlphaBlend(alphaBlend);

  // Program
  auto vs = File("asset://shader/gl_lighting.vert").read();
  auto fs = File("asset://shader/gl_lighting.frag").read();
  auto program = std::make_shared<OpenGLProgram>();
  program->initialize(vs, fs);

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

  // Buffer
  BufferDescription bufferDesc = {
    .object = loadObj(File("asset://model/suzanne/suzanne.obj"))
  };
  auto buffer = device->createBuffer(bufferDesc);

  PipelineDescription pipelineDesc = {
    .buffer = buffer,
    .shaders = {},
    .program = program,
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

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYUP: {
          if (event.key.keysym.sym == SDLK_ESCAPE)
            running = false;
          break;
        }
      }
    }

    CommandQueueDescription queueDesc{};
    auto cmdQueue = device->createCommandQueue(queueDesc);

    CommandBufferDescription cmdBufDesc{};
    auto cmdBuf = cmdQueue->createCommandBuffer(cmdBufDesc);
    renderer->update();
    renderer->render(cmdBuf);

    auto openGLRenderPass = SAFE_DOWN_CAST(OpenGLRenderPass*, renderer->getRenderPass());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLRenderPass->getFrameBuffer(0).getHandle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
      0, 0, width, height, // 소스 영역 (FBO)
      0, 0, width, height, // 대상 영역 (화면)
      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
