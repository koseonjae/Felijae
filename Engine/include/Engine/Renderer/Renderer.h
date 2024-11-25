#pragma once

#include <memory>

namespace goala {

class Scene;
class CommandBuffer;
class RenderPass;

class Renderer : public std::enable_shared_from_this<Renderer> {
 public:
  virtual ~Renderer() = default;

  virtual void update() = 0;

  virtual void render(std::shared_ptr<CommandBuffer> cmdBuf) = 0;

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

} // namespace goala
