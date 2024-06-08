#include "OpenGL/OpenGLPipeline.h"
#include "OpenGL/OpenGLModel.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

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

  // Object setup
  Object obj{};
  Vertex v{};
  v.pos = glm::vec3(1.0, -1.0, 0.0);
  obj.vertices.push_back(v);
  v.pos = glm::vec3(0.0, 1.0, 0.0);
  obj.vertices.push_back(v);
  v.pos = glm::vec3(-1.0, -1.0, 0.0);
  obj.vertices.push_back(v);
  obj.indices.push_back(0);
  obj.indices.push_back(1);
  obj.indices.push_back(2);
  OpenGLModel model;
  model.initialize(move(obj));

  pipeline.program.initialize("../asset/pass_through.vert", "../asset/color.frag");

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
