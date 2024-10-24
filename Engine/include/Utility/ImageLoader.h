#pragma once

#include <vector>
#include <string_view>

enum ImageFormat {
  RGB = 0,
  RGBA = 1,
};

struct ImageData {
  std::vector<uint8_t> pixel;
  int width = 0;
  int height = 0;
  int nChannels = 0;
  ImageFormat format = RGB;
};

class ImageLoader {
 public:
  static ImageData load(std::string_view path);
};
