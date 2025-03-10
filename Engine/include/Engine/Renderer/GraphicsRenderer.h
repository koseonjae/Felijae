#pragma once

#include <Engine/Renderer/Renderer.h>

#include <memory>

namespace larco {
class Scene;
class RenderPass;

class GraphicsRenderer : public Renderer, public std::enable_shared_from_this<GraphicsRenderer> {
public:
  void setRenderPass(std::shared_ptr<RenderPass> renderPass);

  void setScene(std::shared_ptr<Scene> scene);

  Scene* getScene();

  const Scene* getScene() const;

  RenderPass* getRenderPass();

  const RenderPass* getRenderPass() const;

protected:
  std::shared_ptr<Scene> m_scene;
  std::shared_ptr<RenderPass> m_renderPass;
};
} // namespace larco
