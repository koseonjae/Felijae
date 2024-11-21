#pragma once

#include <Base/ImageData.h>
#include <Base/File/File.h>

#include <string_view>

class ImageLoader {
 public:
  static ImageData load(File path);
};
