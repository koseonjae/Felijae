#pragma once

#include <Utility/MetalRef.h>

#include <Metal/MTLDevice.hpp>

class MetalDevice {
 public:
  explicit MetalDevice(MTL::Device* device);
  MTL::Device* get() const;

 private:
  MetalRef<MTL::Device> m_device;
};
