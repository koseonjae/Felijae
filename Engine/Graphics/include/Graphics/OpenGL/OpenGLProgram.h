#pragma once

#include <Base/Utility/ThreadChecker.h>
#include <Graphics/Model/Uniforms.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

#include <string_view>
#include <unordered_map>

namespace goala {
class OpenGLProgram final {
public:
  OpenGLProgram(std::string_view vsPath, std::string_view fsPath);
  ~OpenGLProgram();

  OpenGLProgram(OpenGLProgram&&) = delete;
  OpenGLProgram& operator=(OpenGLProgram&&) = delete;

  OpenGLProgram(const OpenGLProgram&) = delete;
  OpenGLProgram& operator=(const OpenGLProgram&) = delete;

  void bind(Uniforms* uniforms);

private:
  void _updateUniforms(const UniformVariables& uniforms);
  void _updateTextures(TextureVariables textures);
  void _parseShaders();

private:
  GLuint m_program = 0;
  bool m_initialized = false;

  ThreadChecker m_threadChecker;
  TextureVariables m_textureVariables;
  std::unordered_map<std::string, std::string> m_uniformVariablesName;
};
} // namespace goala
