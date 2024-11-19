#pragma once

#include <Utility/MetalRef.h>
#include <Metal/MetalDevice.h>

#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

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
