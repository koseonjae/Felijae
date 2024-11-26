#include <Graphics/Metal/MetalDevice.h>

using namespace goala;

MetalDevice::MetalDevice(MTL::Device* device) : m_device{device} {}

MTL::Device* MetalDevice::getMTLDevice() { return m_device.get(); }

const MTL::Device* MetalDevice::getMTLDevice() const { return m_device.get(); }
