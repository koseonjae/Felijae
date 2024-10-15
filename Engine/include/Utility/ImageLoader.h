#pragma once

#include <vector>
#include <string_view>

enum ImageForat {
  RGB = 0,
  RGBA = 1,
};

struct ImageData {
  std::vector<uint8_t> pixel;
  int width;
  int heigth;
  int nChannels;
  ImageForat format;
};

class ImageLoader {
 public:
  static ImageData load(std::string_view path);
};
