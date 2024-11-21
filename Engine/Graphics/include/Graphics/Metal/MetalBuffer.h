#pragma once

#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Utility/MetalRef.h>

#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

namespace goala {

class Object;

class MetalBuffer {
 public:
  MetalBuffer(MetalDevice* device, const Object& obj);

  MTL::Buffer* getVertexHandle() const;
  MTL::Buffer* getIndexHandle() const;
  MTL::VertexDescriptor* getVertexDescriptor() const;

 private:
  MetalRef<MTL::Buffer> m_vertexHandle;
  MetalRef<MTL::Buffer> m_indexHandle;
  MetalRef<MTL::VertexDescriptor> m_vertexDesc;
};

} // namespace goala
