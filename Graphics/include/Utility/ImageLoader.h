#pragma once

#include <Utility/ImageData.h>

#include <string_view>

class ImageLoader {
 public:
  static ImageData load(std::string_view path);
};
