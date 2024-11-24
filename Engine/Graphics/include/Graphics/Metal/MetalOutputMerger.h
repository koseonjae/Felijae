#pragma once

#include <Graphics/Model/OutputMerger.h>

namespace goala {

class MetalOutputMerger : public OutputMerger {
 public:
  void bind() override;
};

} // namespace goala
