#pragma once

#include <Graphics/Model/Texture.h>

#include <glm/glm.hpp>

#include <mutex>
#include <unordered_map>
#include <string_view>
#include <functional>
#include <tuple>

namespace larco {
using UniformType = std::variant<int, float, glm::vec3, glm::mat4>;
using UniformVariables = std::unordered_map<std::string, UniformType>;
using TextureVariables = std::unordered_map<std::string, std::shared_ptr<Texture>>;

std::tuple<const void*, int> getUniformAddress(const UniformType& variable);

class Uniforms {
public:
  void setUniform(std::string_view name, UniformType variable);

  void setTexture(std::string_view name, std::shared_ptr<Texture> texture);

  std::unordered_map<std::string, UniformType>& getUniforms();

  const std::unordered_map<std::string, UniformType>& getUniforms() const;

  const TextureVariables& getTextures() const;

  TextureVariables& getTextures();

private:
  mutable std::mutex m_uniformsLock;
  mutable std::mutex m_texturesLock;
  UniformVariables m_uniforms;
  TextureVariables m_textures;
};
} // namespace larco
