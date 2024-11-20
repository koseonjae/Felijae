#include <OpenGL/OpenGLPipeline.h>
#include <OpenGL/OpenGLModel.h>
#include <OpenGL/OpenGLProgram.h>
#include <OpenGL/OpenGLTexture.h>
#include <OpenGL/OpenGLBuffer.h>
#include "Model/Triangle.h"
#include <Model/Light.h>
#include <Model/Scene.h>
#include <Utility/FileReader.h>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <memory>

using namespace std;

auto pipeline = make_shared<OpenGLPipeline>();

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

  int width, height;
  SDL_GL_GetDrawableSize(window, &width, &height);
  pipeline->getRasterizer().getViewport().setViewport(0, 0, width, height);

  pipeline->getOutputMerger().getDepthTest().setEnable(true);
  pipeline->getOutputMerger().getDepthTest().setDepthFunc(DepthTest::DepthTestFunc::Less);

  pipeline->getOutputMerger().getAlphaBlend().setEnable(true);
  pipeline->getOutputMerger().getAlphaBlend().setFragmentBlendFunc(AlphaBlend::BlendFunc::SRC_ALPHA);
  pipeline->getOutputMerger().getAlphaBlend().setPixelBlendFunc(AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA);
  pipeline->getOutputMerger().getAlphaBlend().setBlendEquation(AlphaBlend::BlendEquation::Add);

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

  auto worldMat = glm::mat4(1.0);

  auto vs = readFile("../asset/shader/lighting.vert");
  auto fs = readFile("../asset/shader/lighting.frag");
  auto program = std::make_unique<OpenGLProgram>();
  program->initialize(vs, fs);
  program->setUniform("uWorldMat", worldMat);

  Scene scene;
  {
    Light light{};
    light.setLightColor({1.0f, 1.0f, 1.0f});
    light.setLightDirection({0.0f, 0.0f, 1.0f});
    scene.setLight(light);
  }
  {
    glm::vec3 eye = glm::vec3(3.0, 3.0, 3.0);
    glm::vec3 at = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    auto fovy = glm::radians<float>(90);
    auto aspectRatio = 1.f; // frustum width == height
    float n = 0.1f; // note: 0이면 투영행렬 정의상 분모가 0이되어 미정의 동작,
    //                 0.01, 0.001등 너무 작으면 정밀도가 낮아져 가까운 물체들에 대해 z fighting 발생할 수 있음
    //                 => depth bit 조절 필요
    float f = 100.0f;
    Camera camera{};
    camera.setCamera(eye, at, up);
    camera.setProjection(fovy, aspectRatio, n, f);
    scene.setCamera(camera);
  }

  glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
  program->setUniform("uEmitLight", emitLight);

  auto texture = make_shared<OpenGLTexture>("../asset/model/suzanne/uvmap.jpeg");
  program->setTexture("uTexture", texture);

  auto obj = loadObj("../asset/model/suzanne/suzanne.obj");

  auto buffer = std::make_shared<OpenGLBuffer>();
  buffer->initialize(obj);

  auto model = std::make_shared<OpenGLModel>();
  model->setBuffer(buffer);
  model->setPipeline(pipeline);
  model->setProgram(std::move(program));
  scene.addModel(std::move(model));

  // todo: clear, clearColor -> framebuffer class
  glClearColor(0.0, 1.0, 0.0, 0.0);
  glClearDepth(1.0f);

  bool running = true;
  while (running) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
