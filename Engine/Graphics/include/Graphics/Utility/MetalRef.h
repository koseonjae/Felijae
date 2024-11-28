#pragma once

#include <memory>

namespace goala {
template <typename T>
using MetalRef = std::shared_ptr<T>;

template <typename T>
MetalRef<T> makeMetalRef(T* ptr) {
  ptr->retain();
  auto sharedPtr = std::shared_ptr<T>(ptr, [](T* ptr) { ptr->release(); });
  return sharedPtr;
}
} // namespace goala
