#include <Engine/Model/Scene.h>
#include <Engine/Model/Model.h>
#include <Engine/Renderer/Renderer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/Program.h>

void Scene::update() {
  m_renderer->update();
}

void Scene::render() {
  m_renderer->render();
}

void Scene::setLight(const Light& light) {
  static_assert(std::is_trivially_copyable_v<Light> && "Light must be trivially copyable");
  m_light = light;
}

void Scene::setCamera(const Camera& camera) {
  static_assert(std::is_trivially_copyable_v<Camera> && "Camera must be trivially copyable");
  m_camera = camera;
}

void Scene::setRenderer(std::unique_ptr<Renderer> renderer) {
  m_renderer = std::move(renderer);
  m_renderer->setScene(this);
}

void Scene::addModel(std::shared_ptr<Model> model) {
  m_models.push_back(std::move(model));
}

const Light& Scene::getLight() const {
  return m_light;
}

const Camera& Scene::getCamera() const {
  return m_camera;
}

const std::vector<std::shared_ptr<Model>>& Scene::getModels() const {
  return m_models;
}

std::vector<std::shared_ptr<Model>>& Scene::getModels() {
  return m_models;
}
