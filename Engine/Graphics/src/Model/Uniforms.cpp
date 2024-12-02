#include <Graphics/Model/Uniforms.h>

using namespace goala;

void Uniforms::setUniform(std::string_view name, UniformType variable) {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  m_uniforms.insert({name.data(), std::move(variable)});
}

void Uniforms::setTexture(std::string_view name, std::shared_ptr<Texture> texture) {
  std::lock_guard<std::mutex> l(m_texturesLock);
  m_textures.insert({name.data(), std::move(texture)});
}

UniformVariables Uniforms::getUniforms() {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  auto uniforms = std::move(m_uniforms);
  return uniforms;
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
