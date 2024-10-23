#include "OpenGL/OpenGLPipeline.h"
#include "OpenGL/OpenGLModel.h"
#include "OpenGL/OpenGLProgram.h"
#include "OpenGL/OpenGLTexture.h"
#include "Model/Triangle.h"
#include <Model/Light.h>
#include <Model/Scene.h>
#include <Utility/FileReader.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

using namespace std;

auto pipeline = make_shared<OpenGLPipeline>();

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  // Create GLFW window
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  pipeline->getRasterizer().getViewport().setViewport(0, 0, width, height);

  pipeline->getOutputMerger().getDepthTest().setEnable(true);
  pipeline->getOutputMerger().getDepthTest().setDepthFunc(DepthTest::DepthTestFunc::Less);

  pipeline->getOutputMerger().getAlphaBlend().setEnable(true);
  pipeline->getOutputMerger().getAlphaBlend().setFragmentBlendFunc(AlphaBlend::BlendFunc::SRC_ALPHA);
  pipeline->getOutputMerger().getAlphaBlend().setPixelBlendFunc(AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA);
  pipeline->getOutputMerger().getAlphaBlend().setBlendEquation(AlphaBlend::BlendEquation::Add);

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    pipeline->getRasterizer().getViewport().setViewport(0, 0, width, height);
  });

  auto worldMat = glm::mat4(1.0);

  auto vs = readFileToString("../asset/shader/lighting.vert");
  auto fs = readFileToString("../asset/shader/lighting.frag");
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

  auto model = std::make_shared<OpenGLModel>();
  model->initialize(obj);
  model->setPipeline(pipeline);
  model->setProgram(std::move(program));
  scene.addModel(std::move(model));

  // todo: clear, clearColor -> framebuffer class
  glClearColor(0.0, 1.0, 0.0, 0.0);
  glClearDepthf(1.0f);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene.update();
    scene.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
