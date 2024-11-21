#pragma once

#include <Graphics/Model/Model.h>
#include <Graphics/Model/Object.h>

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string_view>
#include <memory>

class OpenGLPipeline;
class OpenGLProgram;

class OpenGLModel final : public Model {
 public:
  OpenGLModel() = default;

  void update() override;

  void render() override;
};
