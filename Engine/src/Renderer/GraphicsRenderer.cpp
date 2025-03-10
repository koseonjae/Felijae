#include <Engine/Renderer/GraphicsRenderer.h>

namespace larco {
void GraphicsRenderer::setScene(std::shared_ptr<Scene> scene) {
  m_scene = std::move(scene);
}

void GraphicsRenderer::setRenderPass(std::shared_ptr<RenderPass> renderPass) {
  m_renderPass = std::move(renderPass);
}

Scene* GraphicsRenderer::getScene() {
  return m_scene.get();
}

const Scene* GraphicsRenderer::getScene() const {
  return m_scene.get();
}

RenderPass* GraphicsRenderer::getRenderPass() {
  return m_renderPass.get();
}

const RenderPass* GraphicsRenderer::getRenderPass() const {
  return m_renderPass.get();
}
} // namespace larco
