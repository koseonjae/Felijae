#include <Engine/Renderer/Renderer.h>

namespace goala {
void Renderer::setScene(std::shared_ptr<Scene> scene) {
  m_scene = std::move(scene);
}
} // namespace goala