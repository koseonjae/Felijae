#pragma once

#include <Graphics/Model/OutputMerger.h>

namespace goala {
class OpenGLOutputMerger : public OutputMerger {
public:
  void bind(void* descriptor) override;
  void encode(CommandEncoder* encoder) override;

private:
  void _bindDepthTest();
  void _bindAlphaBlending();
};
} // namespace goala
