#include <Engine/Renderer/Renderer.h>

namespace goala {
void Renderer::setScene(std::shared_ptr<Scene> scene) {
  m_scene = std::move(scene);
}

void Renderer::setRenderPass(std::shared_ptr<RenderPass> renderPass) {
  m_renderPass = std::move(renderPass);
}

Scene* Renderer::getScene() {
  return m_scene.get();
}

const Scene* Renderer::getScene() const {
  return m_scene.get();
}

RenderPass* Renderer::getRenderPass() {
  return m_renderPass.get();
}

const RenderPass* Renderer::getRenderPass() const {
  return m_renderPass.get();
}
} // namespace goala
