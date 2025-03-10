#pragma once

#include <Foundation/Foundation.hpp>

namespace larco {
class ARCManager final {
public:
  ARCManager() : pPool(NS::AutoreleasePool::alloc()->init()) {};

  ~ARCManager() {
    pPool->release();
  }

private:
  NS::AutoreleasePool* pPool = nullptr;
};
} // namespace larco
