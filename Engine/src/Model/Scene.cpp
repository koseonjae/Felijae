#include <Engine/Model/Scene.h>

#include <Graphics/Model/Pipeline.h>

#include <Engine/Model/Model.h>
#include <Engine/Renderer/Renderer.h>

using namespace goala;

void Scene::addModel(std::shared_ptr<Model> model) {
  m_models.push_back(std::move(model));
}

const std::vector<std::shared_ptr<Model>>& Scene::getModels() const {
  return m_models;
}

std::vector<std::shared_ptr<Model>>& Scene::getModels() {
  return m_models;
}
