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

UniformVariables Uniforms::retrieveUniforms() {
  std::lock_guard<std::mutex> l(m_uniformsLock);
  auto uniforms = std::move(m_uniforms);
  return uniforms;
}

TextureVariables Uniforms::retrieveTextures() {
  std::lock_guard<std::mutex> l(m_texturesLock);
  auto textures = std::move(m_textures);
  return textures;
}
