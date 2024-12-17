#pragma once

#include <Foundation/Foundation.hpp>

namespace goala {
class ARCManager final {
public:
  ARCManager() : pPool(NS::AutoreleasePool::alloc()->init()) {};

  ~ARCManager() {
    pPool->release();
  }

private:
  NS::AutoreleasePool* pPool = nullptr;
};
} // namespace goala
