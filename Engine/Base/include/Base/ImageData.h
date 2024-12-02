#pragma once

#include <Base/ImageFormat.h>

#include <vector>

namespace goala {
struct ImageData {
  std::vector<uint8_t> pixel;
  int width = -1;
  int height = -1;
  int channels = -1;
  ImageFormat format = ImageFormat::UNDEFINED;
};
} // namespace goala
