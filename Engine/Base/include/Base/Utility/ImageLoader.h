#pragma once

#include <Base/File/File.h>
#include <Base/ImageData.h>

#include <string_view>

namespace larco {
class ImageLoader {
public:
  static ImageData load(File path);
};
} // namespace larco
