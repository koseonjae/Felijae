#pragma once

#include <Engine/Model/Scene.h>

#include <memory>

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual void update() = 0;

  virtual void render() = 0;

  void setScene(Scene* scene) { m_scene = scene; }

 protected:
  Scene* m_scene = nullptr;
};
