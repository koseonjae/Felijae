#pragma once

#include <Graphics/Model/VertexBuffer.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class Buffer;
class VertexDescriptor;
}

namespace larco {
constexpr int VERTEX_BUFFER_INDEX = 10;

class MetalDevice;

class MetalVertexBuffer : public VertexBuffer {
public:
  MetalVertexBuffer(MetalDevice* device, VertexBufferDescription obj);

  MTL::Buffer* getVertexHandle();
  const MTL::Buffer* getVertexHandle() const;

  MTL::Buffer* getIndexHandle();
  const MTL::Buffer* getIndexHandle() const;

  MTL::VertexDescriptor* getVertexDescriptor();
  const MTL::VertexDescriptor* getVertexDescriptor() const;
  int getIndicesSize() const;

private:
  MetalRef<MTL::Buffer> m_vertexHandle;
  MetalRef<MTL::Buffer> m_indexHandle;
  MetalRef<MTL::VertexDescriptor> m_vertexDesc;

  int m_indicesSize = 0;
};
} // namespace larco
