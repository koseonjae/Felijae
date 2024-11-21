#pragma once

#include <Base/ImageFormat.h>

#include <vector>

namespace goala {

struct ImageData {
  std::vector<uint8_t> pixel;
  int width = 0;
  int height = 0;
  ImageFormat format = ImageFormat::RGB;
};

} // namespace goala
