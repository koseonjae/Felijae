#pragma once

#include <Base/ImageData.h>

namespace goala {
enum class TextureLoadType {
  EAGER, LAZY
};

struct TextureDescription {
  ImageData imageData{};
  TextureLoadType loadType = TextureLoadType::EAGER;
};

class Texture {
public:
  virtual ~Texture() = default;

  virtual void bind() = 0;
};
} // namespace goala
