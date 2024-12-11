#pragma once

namespace MTL {
class Texture;
}

namespace goala {
struct ImageData;
class MetalDevice;

class FormatConverter {
public:
  static void rgb2bgra(MetalDevice* device, const ImageData& image, MTL::Texture* targetTexture);
};
} // namespace goala
