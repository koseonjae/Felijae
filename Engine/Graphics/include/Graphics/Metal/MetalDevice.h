#pragma once

#include <Graphics/Utility/MetalRef.h>

#include <Metal/MTLDevice.hpp>

namespace goala {

class MetalDevice {
 public:
  explicit MetalDevice(MTL::Device* device);
  MTL::Device* get() const;

 private:
  MetalRef<MTL::Device> m_device;
};

} // namespace goala
