#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLModel.h>
#include <Graphics/OpenGL/OpenGLProgram.h>
#include <Graphics/OpenGL/OpenGLTexture.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <Graphics/OpenGL/OpenGLRasterizer.h>
#include <Graphics/OpenGL/OpenGLOutputMerger.h>
#include <Graphics/Model/Triangle.h>
#include <Graphics/Model/Light.h>
#include <Graphics/Model/Scene.h>
#include <Graphics/Utility/FileReader.h>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <memory>

using namespace std;

int main() {
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

  SDL_AddEventWatch([](void* data, SDL_Event* event) -> int {
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

  auto pipeline = make_shared<OpenGLPipeline>();

  // Rasterizer
  {
    Culling culling = {
        .enable = true,
        .frontFace = Culling::FrontFace::CCW,
        .cullMode = Culling::CullMode::Back
    };
    Viewport viewport = {
        .minX = 0,
        .minY = 0,
        .width = width,
        .height = height,
        .minZ = 0.0f,
        .maxZ = 1.0f
    };
    auto rasterizer = std::make_shared<OpenGLRasterizer>();
    rasterizer->setCulling(culling);
    rasterizer->setViewport(viewport);
    pipeline->setRasterizer(std::move(rasterizer));
  }

  // Output Merger
  {
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
    pipeline->setOutputMerger(std::move(outputMerger));
  }

  // Program
  {
    auto vs = readFile("../asset/shader/lighting.vert");
    auto fs = readFile("../asset/shader/lighting.frag");
    auto program = std::make_shared<OpenGLProgram>();
    program->initialize(vs, fs);

    glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
    program->setUniform("uEmitLight", emitLight);

    auto texture = std::make_shared<OpenGLTexture>();
    texture->initialize("../asset/model/suzanne/uvmap.jpeg");
    program->setTexture("uTexture", texture);

    pipeline->setProgram(program);
  }

  // Buffer
  {
    auto obj = loadObj("../asset/model/suzanne/suzanne.obj");
    auto buffer = std::make_shared<OpenGLBuffer>();
    buffer->initialize(obj);
    pipeline->setBuffer(std::move(buffer));
  }

  // Render Pass
  {
    auto colorTexture = std::make_shared<OpenGLTexture>();
    colorTexture->initialize(width, height, ImageFormat::RGBA);

    std::vector<Attachment> attachments;
    attachments.emplace_back(Attachment{
        .type = AttachmentType::Color,
        .loadFunc = LoadFunc::Clear,
        .storeFunc = StoreFunc::Store,
        .clear = ClearColor{1.0f, 1.0f, 0.0f, 1.0f},
        .texture = std::move(colorTexture),
    });

    auto renderPass = std::make_shared<OpenGLRenderPass>();
    renderPass->setAttachments(std::move(attachments));
    pipeline->setRenderPass(std::move(renderPass));
  }

  // Scene
  Scene scene;

  // Light
  {
    Light light{};
    light.setLightColor({1.0f, 1.0f, 1.0f});
    light.setLightDirection({0.0f, 0.0f, 1.0f});
    scene.setLight(light);
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
    Camera camera{};
    camera.setCamera(eye, at, up);
    camera.setProjection(fovy, aspectRatio, n, f);
    scene.setCamera(camera);
  }

  // Model
  {
    auto model = std::make_shared<OpenGLModel>();
    model->setPipeline(pipeline);
    scene.addModel(std::move(model));
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

    scene.update();
    scene.render();

    auto openGLRenderPass = dynamic_cast<OpenGLRenderPass*>(pipeline->getRenderPass());
    assert(openGLRenderPass != nullptr && "Failed to cast to openGLRenderPass");
    glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLRenderPass->getFrameBuffer(0).getHandle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, width, height,  // 소스 영역 (FBO)
        0, 0, width, height,  // 대상 영역 (화면)
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
