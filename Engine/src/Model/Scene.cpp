#include <Engine/Model/Scene.h>

#include <Graphics/Model/Pipeline.h>

#include <Engine/Model/Model.h>
#include <Engine/Renderer/Renderer.h>

using namespace goala;

void Scene::setLight(const Light& light) {
  static_assert(std::is_trivially_copyable_v<Light> && "Light must be trivially copyable");
  m_light = light;
}

void Scene::setCamera(const Camera& camera) {
  static_assert(std::is_trivially_copyable_v<Camera> && "Camera must be trivially copyable");
  m_camera = camera;
}

void Scene::addModel(std::shared_ptr<Model> model) {
  m_models.push_back(std::move(model));
}

const Light& Scene::getLight() const { return m_light; }

const Camera& Scene::getCamera() const { return m_camera; }

const std::vector<std::shared_ptr<Model>>& Scene::getModels() const {
  return m_models;
}

std::vector<std::shared_ptr<Model>>& Scene::getModels() { return m_models; }
