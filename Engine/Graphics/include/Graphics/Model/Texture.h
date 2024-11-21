#pragma once

#include <Graphics/Utility/ImageData.h>

#include <string_view>

#include <cstdint>

class Texture {
 public:
  virtual ~Texture() = default;

  virtual void initialize(std::string_view path, bool lazyLoading) = 0;

  virtual void initialize(int width, int height, ImageFormat format, bool lazyLoading) = 0;

  virtual void initialize(ImageData imageData, bool lazyLoading) = 0;

  virtual void bind() = 0;

  virtual uint32_t getHandle() const = 0;
};
