#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

using namespace std;

struct Vertex {
  glm::vec3 pos;
};

struct Object {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

std::string readFileToString(const std::filesystem::path &filePath) {
  std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
  if (!fileStream) {
    throw std::runtime_error("Could not open file: " + filePath.string());
  }

  std::string content;
  fileStream.seekg(0, std::ios::end);
  content.resize(fileStream.tellg());
  fileStream.seekg(0, std::ios::beg);
  fileStream.read(&content[0], content.size());
  fileStream.close();

  return content;
}

void checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}

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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

  // VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // VBO
  GLuint abo;
  glGenBuffers(1, &abo);
  glBindBuffer(GL_ARRAY_BUFFER, abo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj.vertices.size(), obj.vertices.data(), GL_STATIC_DRAW);

  // EBO
  GLuint eabo;
  glGenBuffers(1, &eabo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj.indices.size(), obj.indices.data(), GL_STATIC_DRAW);

  // Vertex attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));

  // Load shaders
  auto vertexShaderStr = readFileToString("../asset/pass_through.vert");
  auto fragShaderStr = readFileToString("../asset/color.frag");
  const char *vs_str = vertexShaderStr.c_str();
  const char *fs_str = fragShaderStr.c_str();

  // Compile vertex shader
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_str, nullptr);
  glCompileShader(vs);
  checkCompileErrors(vs, "VERTEX");

  // Compile fragment shader
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_str, nullptr);
  glCompileShader(fs);
  checkCompileErrors(fs, "FRAGMENT");

  // Link shaders into program
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  checkCompileErrors(program, "PROGRAM");

  glDeleteShader(vs);
  glDeleteShader(fs);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
