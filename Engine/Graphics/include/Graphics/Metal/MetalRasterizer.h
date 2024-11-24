#pragma once

#include <Graphics/Model/Rasterizer.h>

namespace goala {

class MetalRasterizer : public Rasterizer {
 public:
  void bind() override;
};

} // namespace goala
