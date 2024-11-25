#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

#include <mutex>
#include <unordered_map>
#include <string_view>
#include <functional>

namespace goala {

using UniformType = std::variant<glm::vec3, glm::mat4>;
using UniformVariables = std::unordered_map<std::string, UniformType>;
using TextureVariables = std::unordered_map<std::string, std::shared_ptr<Texture>>;

class Uniforms {
 public:
  void setUniform(std::string_view name, UniformType variable);

  void setTexture(std::string_view name, std::shared_ptr<Texture> texture);

  std::unordered_map<std::string, UniformType> retrieveUniforms();

  std::unordered_map<std::string, std::shared_ptr<Texture>> retrieveTextures();

 private:
  std::mutex m_uniformsLock;
  std::mutex m_texturesLock;
  UniformVariables m_uniforms;
  TextureVariables m_textures;
};

} // namespace goala
