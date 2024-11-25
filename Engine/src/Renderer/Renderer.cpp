#include <Engine/Renderer/Renderer.h>

namespace goala {

void Renderer::setScene(std::shared_ptr<Scene> scene) {
  m_scene = std::move(scene);
}

Scene* Renderer::getScene() {
  return m_scene.get();
}

const Scene* Renderer::getScene() const {
  return m_scene.get();
}

} // namespace goala
