#include "OpenGL/OpenGLPipeline.h"
#include "OpenGL/OpenGLModel.h"
#include "OpenGL/OpenGLProgram.h"
#include "OpenGL/OpenGLTexture.h"
#include "Model/Triangle.h"
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

  // Create GLFW window
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
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
  pipeline->rasterizer.viewport.setViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    pipeline->rasterizer.viewport.setViewport(0, 0, width, height);
  });

  glClearColor(0.0, 1.0, 0.0, 1.0);

  auto worldMat = glm::mat4(1.0);

  glm::vec3 eye = glm::vec3(3.0, 3.0, 3.0);
  glm::vec3 at = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
  auto viewMat = glm::lookAt(eye, at, up);

  auto fovy = glm::radians<float>(90);
  auto aspectRatio = 1.f; // frustum width == height
  float n = 0.0f;
  float f = 100.0f;
  auto projMat = glm::perspective(fovy, aspectRatio, n, f);

  auto vs = readFileToString("../asset/shader/lighting.vert");
  auto fs = readFileToString("../asset/shader/lighting.frag");
  auto program = std::make_unique<OpenGLProgram>();
  program->initialize(vs, fs);
  program->setUniform("uWorldMat", worldMat);
  program->setUniform("uViewMat", viewMat);
  program->setUniform("uProjMat", projMat);
  program->setUniform("uCameraPosition", eye);

  glm::vec3 lightDir{0.0f, 0.0f, 5.0f};
  glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
  program->setUniform("uLightDir", lightDir);
  program->setUniform("uLightColor", lightColor);

  glm::vec3 emitLight{0.0f, 0.0f, 0.0f};
  program->setUniform("uEmitLight", emitLight);

  auto texture = make_shared<OpenGLTexture>("../asset/model/suzanne/uvmap.jpeg");
  program->setTexture("uTexture", texture);

  auto obj = loadObj("../asset/model/suzanne/suzanne.obj");

  OpenGLModel model;
  model.initialize(std::move(obj));
  model.setPipeline(pipeline);
  model.setProgram(std::move(program));

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    model.update();
    model.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
