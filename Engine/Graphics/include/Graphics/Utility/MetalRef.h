#pragma once

#include <memory>

namespace larco {
template <typename T>
using MetalRef = std::shared_ptr<T>;

template <typename T>
MetalRef<T> makeMetalRef(T* ptr) {
  auto sharedPtr = std::shared_ptr<T>(ptr->retain(), [](T* ptr) { ptr->release(); });
  return sharedPtr;
}
} // namespace larco
