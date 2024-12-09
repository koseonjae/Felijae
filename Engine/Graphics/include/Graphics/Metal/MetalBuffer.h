#pragma once

#include <Graphics/Model/Buffer.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class Buffer;
class VertexDescriptor;
}

namespace goala {
constexpr int VERTEX_BUFFER_INDEX = 10;

class MetalDevice;

class MetalBuffer : public Buffer {
public:
  MetalBuffer(MetalDevice* device, VertexBufferDescription obj);

  void bind() override;

  void draw() override;

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
} // namespace goala
