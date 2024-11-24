#pragma once

#include <Base/File/File.h>
#include <Base/ImageData.h>

#include <cstdint>
#include <string_view>
#include <variant>

namespace MTL {
class Texture;
}

namespace goala {

using TextureHandle = std::variant<uint32_t, MTL::Texture*>;

class Texture {
 public:
  virtual ~Texture() = default;

  virtual void initialize(File path, bool lazyLoading) = 0;

  virtual void initialize(int width, int height, ImageFormat format, bool lazyLoading) = 0;

  virtual void initialize(ImageData imageData, bool lazyLoading) = 0;

  template <typename T>
  void initializeExternal(T externalHandle) {
    m_handle = externalHandle;
    m_externalHandleInitialized = true;
  }

  virtual void bind() = 0;

  template<typename T>
  T getHandle() const {
    return std::get<T>(m_handle);
  }

 protected:
  TextureHandle m_handle{};
  bool m_externalHandleInitialized = false;
};

} // namespace goala
