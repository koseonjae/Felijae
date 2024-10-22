#include <Model/Scene.h>
#include <Model/Model.h>

void Scene::setLight(const Light &light) {
  static_assert(std::is_trivially_copyable_v<Light> && "Light must be trivially copyable");
  m_light = light;
}

void Scene::setCamera(const Camera &camera) {
  static_assert(std::is_trivially_copyable_v<Camera> && "Camera must be trivially copyable");
  m_camera = camera;
}

void Scene::addModel(std::shared_ptr<Model> model) {
  m_models.push_back(std::move(model));
}

void Scene::update() {
  for(auto& model : m_models) {
    model->update();
  }
}

void Scene::render() {
  for(auto& model : m_models) {
    model->render();
  }
}