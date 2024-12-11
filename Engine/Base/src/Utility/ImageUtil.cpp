#include <Base/Utility/ImageUtil.h>

#include <cassert>

namespace goala {
ImageData convertRGB2BGRA(const ImageData& src) {
  // todo: Apply compute shader

  assert(src.channels == 3);
  assert(src.pixelFormat == ImageFormat::RGB);

  ImageData result = {
    .pixel = {},
    .width = src.width,
    .height = src.height,
    .channels = src.channels + 1,
    .pixelFormat = ImageFormat::BGRA,
  };

  const auto& srcPixel = src.pixel;
  auto& dstPixel = result.pixel;
  dstPixel.reserve(result.width * result.height * result.channels);
  for (int i = 0; i < src.width * src.height; ++i) {
    dstPixel.push_back(srcPixel[3 * i + 2]); // B
    dstPixel.push_back(srcPixel[3 * i + 1]); // G
    dstPixel.push_back(srcPixel[3 * i]);     // R
    dstPixel.push_back(255);                 // A
  }

  return result;
}
} // namespace goala
