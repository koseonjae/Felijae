#include <Engine/Renderer/ForwardRenderer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/Model/CommandBuffer.h>
#include <Engine/Model/Model.h>
#include <Engine/Model/Scene.h>

#include "Graphics/Model/CommandEncoder.h"

using namespace goala;

void ForwardRenderer::update() {
  auto updateNodes = [self = shared_from_this()](Uniforms* uniforms) {
    if (!self)
      return;
    if (auto light = self->getScene()->getNode<Light>()) {
      uniforms->setUniform("uLightDir", light->getLightDirection());
      uniforms->setUniform("uLightColor", light->getLightColor());
    }
    if (auto camera = self->getScene()->getNode<Camera>()) {
      uniforms->setUniform("uViewMat", camera->getViewMatrix());
      uniforms->setUniform("uProjMat", camera->getProjMatrix());
      uniforms->setUniform("uCameraPosition", camera->getEye());
    }
  };

  auto& models = m_scene->getModels();
  for (auto& model : models) {
    if (auto* uniforms = model->getPipeline()->getUniforms()) {
      updateNodes(uniforms);
      uniforms->setUniform("uWorldMat", model->calculateWorldMat());
    }
    model->update();
  }

  // m_renderPass->update();
}

void ForwardRenderer::render(std::shared_ptr<CommandBuffer> cmdBuf) {
  auto& models = m_scene->getModels();
  CommandEncoderDescription encoderDesc{};
  auto encoder = cmdBuf->createCommandEncoder(getRenderPass(), encoderDesc);
  for (auto& model : models)
    encoder->encodeDraw(model->getPipeline());
  encoder->endEncoding();
  // offscreen 렌더링하면서 present 호출하지 않도록 변경
  // cmdBuf->present(m_renderPass->getAttachments()[0].texture.get());
}
