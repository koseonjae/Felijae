#include <Graphics/Metal/MetalDevice.h>

using namespace goala;

MetalDevice::MetalDevice(MTL::Device* device) : m_device{device} {}

MTL::Device* MetalDevice::get() const { return m_device.get(); }
