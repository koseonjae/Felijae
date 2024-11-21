#include <Graphics/Model/Scene.h>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/Program.h>

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

void Scene::update() {
  auto lightColor = m_light.getLightColor();
  auto lightDir = m_light.getLightDirection();
  auto view = m_camera.getViewMatrix();
  auto proj = m_camera.getProjMatrix();
  auto eye = m_camera.getEye();
  auto updateProgram = [&](Program* program) {
    program->setUniform("uLightDir", lightDir);
    program->setUniform("uLightColor", lightColor);
    program->setUniform("uViewMat", view);
    program->setUniform("uProjMat", proj);
    program->setUniform("uCameraPosition", eye);
  };

  for (auto& model : m_models) {
    auto* program = model->getPipeline()->getProgram();
    updateProgram(program);
    program->setUniform("uWorldMat", model->calculateWorldMat());
    model->update();
  }
}

void Scene::render() {
  for (auto& model : m_models) {
    model->render();
  }
}
