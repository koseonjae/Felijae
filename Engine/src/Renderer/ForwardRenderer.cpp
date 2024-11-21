#include <Engine/Model/Model.h>
#include <Engine/Renderer/ForwardRenderer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/Program.h>

using namespace goala;

void ForwardRenderer::update() {
  auto lightColor = m_scene->getLight().getLightColor();
  auto lightDir = m_scene->getLight().getLightDirection();
  auto view = m_scene->getCamera().getViewMatrix();
  auto proj = m_scene->getCamera().getProjMatrix();
  auto eye = m_scene->getCamera().getEye();
  auto updateProgram = [&](Program* program) {
    program->setUniform("uLightDir", lightDir);
    program->setUniform("uLightColor", lightColor);
    program->setUniform("uViewMat", view);
    program->setUniform("uProjMat", proj);
    program->setUniform("uCameraPosition", eye);
  };

  auto& models = m_scene->getModels();
  for (auto& model : models) {
    auto* program = model->getPipeline()->getProgram();
    updateProgram(program);
    program->setUniform("uWorldMat", model->calculateWorldMat());
    model->update();
  }
}

void ForwardRenderer::render() {
  auto& models = m_scene->getModels();
  for (auto& model : models) {
    model->render();
  }
}
