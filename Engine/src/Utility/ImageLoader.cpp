#include "Utility/ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cassert>

ImageData ImageLoader::load(std::string_view path) {
  ImageData imageData{};

  // opengl의 텍스쳐 좌표   : 좌하단이 0,0
  // 이미지의 좌표          : 좌상단이 0,0
  // 이를 보정하기 위한 옵션
  stbi_set_flip_vertically_on_load(true);

  unsigned char *data = stbi_load(path.data(), &imageData.width, &imageData.height, &imageData.nChannels, 0);
  assert(data != nullptr && "Failed to load imageData");
  assert(imageData.nChannels == 3);
  imageData.format = ImageFormat::RGB;

  auto size = imageData.width * imageData.height * imageData.nChannels;
  imageData.pixel.resize(size);
  memcpy(imageData.pixel.data(), data, size);

  stbi_image_free(data);

  return imageData;
}
