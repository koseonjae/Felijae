#pragma once

#include <Base/File/File.h>
#include <Base/ImageData.h>

#include <cstdint>
#include <string_view>
#include <variant>

namespace goala {

class Texture {
 public:
  virtual ~Texture() = default;

  virtual void initialize(File path, bool lazyLoading) = 0;

  virtual void initialize(int width, int height, ImageFormat format, bool lazyLoading) = 0;

  virtual void initialize(ImageData imageData, bool lazyLoading) = 0;

  virtual void bind() = 0;
};

} // namespace goala
