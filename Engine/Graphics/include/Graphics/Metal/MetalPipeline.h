#pragma one

#include <Graphics/Model/Pipeline.h>

namespace goala {
class MetalDevice;

class MetalPipeline : public Pipeline {
 public:
  explicit MetalPipeline(MetalDevice* device);

  void update() override;
  void render() override;
};

} // namespace goala
