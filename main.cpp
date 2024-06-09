#include "OpenGL/OpenGLPipeline.h"
#include "OpenGL/OpenGLModel.h"
#include "Model/Triangle.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace std;

OpenGLPipeline pipeline{};

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

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    pipeline.rasterizer.viewport.setViewport(0, 0, width, height);
  });

  glClearColor(0.0, 1.0, 0.0, 1.0);

  auto obj = Triangle::load();

  OpenGLModel model;
  model.initialize(move(obj));

  pipeline.program.initialize("../asset/pass_through.vert", "../asset/color.frag");

  glm::mat4 worldMat(1.0f);
  worldMat = glm::translate(worldMat, glm::vec3(0.0, 0.0, 0.0));

  glm::vec3 eye = glm::vec3(2.0, 0.0, 2.0);
  glm::vec3 at = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
  auto viewMat = glm::lookAt(eye, at, up);

  auto fovy = glm::radians<float>(90);
  auto aspectRatio = 1.f; // frustum width == height
  float n = 0.0f;
  float f = 100.0f;
  auto projMat = glm::perspective(fovy, aspectRatio, n, f);

  pipeline.program.setUniform("worldMat", worldMat);
  pipeline.program.setUniform("viewMat", viewMat);
  pipeline.program.setUniform("projMat", projMat);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    pipeline.bind();
    model.bind();

    model.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
