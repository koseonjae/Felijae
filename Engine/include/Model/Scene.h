#pragma once

#include <Model/Light.h>
#include <Model/Camera.h>

#include <utility>
#include <memory>
#include <vector>

class Model;

class Scene final {
 public:
  void setLight(const Light& light);
  void setCamera(const Camera& camera);
  void addModel(std::shared_ptr<Model> model);
  void update();
  void render();

 private:
  Camera m_camera;
  Light m_light;
  std::vector<std::shared_ptr<Model>> m_models;
};
