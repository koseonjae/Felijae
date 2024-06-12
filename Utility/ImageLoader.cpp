#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cassert>

ImageData ImageLoader::load(std::string_view path) {
  ImageData imageData{};

  unsigned char *data = stbi_load(path.data(), &imageData.width, &imageData.heigth, &imageData.nChannels, 0);
  assert(data != nullptr && "Failed to load imageData");
  assert(imageData.nChannels == 3);
  imageData.format = ImageForat::RGB;

  auto size = imageData.width * imageData.heigth * imageData.nChannels;
  imageData.pixel.resize(size);
  memcpy(imageData.pixel.data(), data, size);

  stbi_image_free(data);

  return imageData;
}