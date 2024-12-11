#pragma once

#include <Base/ImageFormat.h>

#include <vector>

namespace goala {
struct ImageData {
  std::vector<uint8_t> pixel;
  int width = 0;
  int height = 0;
  int channels = 0;
  ImageFormat pixelFormat = ImageFormat::UNDEFINED;
};
} // namespace goala
