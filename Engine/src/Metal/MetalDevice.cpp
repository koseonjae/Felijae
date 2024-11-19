#include <Metal/MetalDevice.h>

MetalDevice::MetalDevice(MTL::Device* device) : m_device{device} {
}

MTL::Device* MetalDevice::get() const {
  return m_device.get();
}
