#pragma once

#include <Engine/Model/Camera.h>
#include <Engine/Model/Light.h>

#include <memory>
#include <utility>
#include <vector>

namespace goala {

class Model;
class Renderer;

class Scene final {
 public:
  void update();
  void render();

  void setLight(const Light& light);
  void setCamera(const Camera& camera);
  void setRenderer(std::unique_ptr<Renderer> renderer);
  void addModel(std::shared_ptr<Model> model);

  const Light& getLight() const;
  const Camera& getCamera() const;
  const std::vector<std::shared_ptr<Model>>& getModels() const;
  std::vector<std::shared_ptr<Model>>& getModels();

 private:
  Camera m_camera; // todo: 얘는 unique_ptr로 변경하되, 없으면 assert(false)를
                   // 만들자
  Light m_light;   // todo: 빛은 있을 수도 있고 없을 수도 있음. unique_ptr로
                   // 변경하자.
  std::vector<std::shared_ptr<Model>> m_models;
  std::unique_ptr<Renderer> m_renderer;
};

} // namespace goala
