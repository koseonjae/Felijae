#pragma once

#include <memory>

namespace goala {

class Scene;

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual void update() = 0;

  virtual void render() = 0;

  void setScene(std::shared_ptr<Scene> scene);

 protected:
  std::shared_ptr<Scene> m_scene;
};

} // namespace goala
