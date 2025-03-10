#include <Graphics/Model/Uniforms.h>

#include <glm/gtc/type_ptr.hpp>

using namespace larco;

namespace larco {
std::tuple<const void*, int> getUniformAddress(const UniformType& variable) {
  const void* valuePtr = nullptr;
  int size = 0;
  std::visit([&valuePtr, &size](auto& value) {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, glm::vec3>) {
      valuePtr = reinterpret_cast<const void*>(glm::value_ptr(value));
      size = sizeof(glm::vec3);
    }
    else if constexpr (std::is_same_v<T, glm::mat4>) {
      valuePtr = reinterpret_cast<const void*>(glm::value_ptr(value));
      size = sizeof(glm::mat4);
    }
    else if constexpr (std::is_same_v<T, float>) {
      valuePtr = reinterpret_cast<const void*>(&value);
      size = sizeof(float);
    }
    else if constexpr (std::is_same_v<T, int>) {
      valuePtr = reinterpret_cast<const void*>(&value);
      size = sizeof(int);
    }
    else
      assert(false && "Unsupported type");
  }, variable);
  return {valuePtr, size};
}
}

void Uniforms::setUniform(std::string_view name, UniformType variable) {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  m_uniforms.insert({name.data(), std::move(variable)});
}

void Uniforms::setTexture(std::string_view name, std::shared_ptr<Texture> texture) {
  std::lock_guard<std::mutex> l(m_texturesLock);
  m_textures.insert({name.data(), std::move(texture)});
}

UniformVariables& Uniforms::getUniforms() {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  return m_uniforms;
}

const std::unordered_map<std::string, UniformType>& Uniforms::getUniforms() const {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  return m_uniforms;
}

const TextureVariables& Uniforms::getTextures() const {
  std::lock_guard<std::mutex> l(m_texturesLock);
  return m_textures;
}

TextureVariables& Uniforms::getTextures() {
  // todo: gl은 한번만 세팅해주느라고 받아와서 move하는데, metal은 그렇지 않고 있음. 이런 차이를 어떻게 통일할지 확인
  std::lock_guard<std::mutex> l(m_texturesLock);
  return m_textures;
}
