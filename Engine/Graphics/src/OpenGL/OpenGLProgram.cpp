#include <Graphics/OpenGL/OpenGLProgram.h>
#include <Graphics/Model/Uniforms.h>

#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string_view>

using namespace goala;

void checkCompileErrors(GLuint shader, std::string_view type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      assert(false && "Shader compilation error");
    }
  }
  else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      assert(false && "program linking error");
    }
  }
}

void OpenGLProgram::initialize(std::string_view vertexShaderStr, std::string_view fragShaderStr) {
  m_threadChecker.checkThread();
  assert(!m_initialized && "OpenGLProgram is already initialized");

  const char* vs_str = vertexShaderStr.data();
  const char* fs_str = fragShaderStr.data();

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
  int program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  checkCompileErrors(program, "PROGRAM");
  m_program = program;

  glDeleteShader(vs);
  glDeleteShader(fs);

  m_initialized = true;
}

OpenGLProgram::~OpenGLProgram() {
  m_threadChecker.checkThread();
  if (!m_initialized)
    return;
  glDeleteProgram(m_program);
}

void OpenGLProgram::bind(Uniforms* uniformVariables) {
  m_threadChecker.checkThread();
  assert(m_initialized && "OpenGLProgram is not initialized");

  glUseProgram(m_program);

  auto uniforms = uniformVariables->getUniforms();
  auto textures = uniformVariables->getTextures();

  _updateUniforms(uniforms);
  _updateTextures(std::move(textures));
}

void OpenGLProgram::_updateUniforms(const UniformVariables& uniforms) {
  for (const auto& [name, variable] : uniforms) {
    GLint loc = glGetUniformLocation(m_program, name.c_str());
    assert(loc != -1 && "Invalid uniform location");

    std::visit([loc](auto& value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, glm::vec3>)
        glUniform3fv(loc, 1, glm::value_ptr(value));
      else if constexpr (std::is_same_v<T, glm::mat4>)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }, variable);
  }
}

void OpenGLProgram::_updateTextures(TextureVariables textures) {
  if (textures.empty())
    return;

  for (auto& [name, texture] : textures)
    m_textureVariables.insert_or_assign(name.data(), std::move(texture));

  int index = 0;
  for (auto& [name, texture] : m_textureVariables) {
    glActiveTexture(GL_TEXTURE0 + index);
    texture->bind();
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1 && "Invalid texture location");
    glUniform1i(loc, index);
    ++index;
  }
}
