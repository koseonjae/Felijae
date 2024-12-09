#include <Graphics/Utility/ImageFormatUtil.h>

namespace goala {
MTL::PixelFormat getMetalImageFormat(ImageFormat format) {
  switch (format) {
    case ImageFormat::Float: // todo: 이걸로 변환하면 되는게 맞나? 속성 확인
      return MTL::PixelFormatRGBA32Float;
    case ImageFormat::RGB:
      assert(false && "RGB is not supported in Metal");
    case ImageFormat::RGBA:
      return MTL::PixelFormatRGBA8Unorm;
    case ImageFormat::BGRA:
      return MTL::PixelFormatBGRA8Unorm;
    default: {}
  }
  assert(false && "Undefined Image Format");
}

ImageFormat getImageFormat(MTL::PixelFormat format) {
  switch (format) {
    case MTL::PixelFormatRGBA8Unorm:
      return ImageFormat::RGBA;
    case MTL::PixelFormatBGRA8Unorm:
      return ImageFormat::BGRA;
    case MTL::PixelFormatRGBA8Unorm_sRGB: // Metal은 기본적으로 RGB 포맷을 직접 지원하지 않으므로, RGB에 해당하는 포맷을 정의적으로 매핑
    case MTL::PixelFormatRGB10A2Unorm:    // 예: sRGB 공간에서의 RGB
      return ImageFormat::RGB;            // 예: RGB + 알파의 10:10:10:2 비트 포맷
    default: {}
  }
  assert(false && "Undefined Image Format");
}
} // namespace goala
