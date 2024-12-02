#include <Base/ImageFormat.h>
#include <Base/Utility/ImageLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>

using namespace goala;

ImageData ImageLoader::load(File file) {
  ImageData imageData{};

  // opengl의 텍스쳐 좌표   : 좌하단이 0,0
  // 이미지의 좌표          : 좌상단이 0,0
  // 이를 보정하기 위한 옵션
  stbi_set_flip_vertically_on_load(true);

  int nChannels = -1;
  auto path = file.getPath();
  unsigned char* data = stbi_load(path.data(), &imageData.width, &imageData.height, &nChannels, 0);
  assert(data != nullptr && "Failed to load imageData");
  assert(nChannels == 3);
  imageData.format = ImageFormat::RGB;
  imageData.channels = nChannels;

  auto size = imageData.width * imageData.height * nChannels;
  imageData.pixel.resize(size);
  memcpy(imageData.pixel.data(), data, size);

  stbi_image_free(data);

  return imageData;
}
