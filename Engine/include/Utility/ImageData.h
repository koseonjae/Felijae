#pragma once

#include <Utility/ImageFormat.h>

#include <vector>

struct ImageData {
  std::vector<uint8_t> pixel;
  int width = 0;
  int height = 0;
  ImageFormat format = ImageFormat::RGB;
};