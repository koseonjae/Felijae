#include <Engine/Renderer/ForwardRenderer.h>

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/Program.h>
#include <Graphics/Model/CommandBuffer.h>

#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>

using namespace goala;

void ForwardRenderer::update() {
  auto updateProgram = [self = shared_from_this()](Program* program) {
    if (!self)
      return;
    if (auto light = self->getScene()->getNode<Light>()) {
      program->setUniform("uLightDir", light->getLightDirection());
      program->setUniform("uLightColor", light->getLightColor());
    }
    if (auto camera = self->getScene()->getNode<Camera>()) {
      program->setUniform("uViewMat", camera->getViewMatrix());
      program->setUniform("uProjMat", camera->getProjMatrix());
      program->setUniform("uCameraPosition", camera->getEye());
    }
  };

  auto& models = m_scene->getModels();
  for (auto& model : models) {
    if (auto* program = model->getPipeline()->getProgram()) {
      // todo: Pipeline에 Uniform 멤버변수 만들고, 거기에 uniform 세팅하자
      updateProgram(program);
      program->setUniform("uWorldMat", model->calculateWorldMat());
    }
    model->update();
  }
}

void ForwardRenderer::render(std::shared_ptr<CommandBuffer> cmdBuf) {
  auto& models = m_scene->getModels();
  for (auto& model : models)
    cmdBuf->encode(model->getPipeline());
  cmdBuf->present(m_renderPass->getAttachments()[0].texture.get()); // todo: pipeline에선 renderpass 빼고, renderer가 가지고 있게 하자
  cmdBuf->commit();
}
