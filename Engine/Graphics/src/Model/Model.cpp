#include <Graphics/Model/Model.h>
#include <Graphics/Model/Pipeline.h>

void Model::update() {
  assert(m_pipeline);
  m_pipeline->update();
}

void Model::render() {
  assert(m_pipeline);
  m_pipeline->render();
}
