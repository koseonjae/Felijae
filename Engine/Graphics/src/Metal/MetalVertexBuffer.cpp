#include <Base/Object/Object.h>
#include <Graphics/Metal/MetalVertexBuffer.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

using namespace larco;

MetalVertexBuffer::MetalVertexBuffer(MetalDevice* device, VertexBufferDescription desc) {
  auto resourceOption = MTL::ResourceCPUCacheModeDefaultCache;

  auto& obj = desc.object;

  auto verticesSize = sizeof(decltype(obj.vertices)::value_type) * obj.vertices.size();
  m_vertexHandle = makeMetalRef(device->getMTLDevice()->newBuffer((void*)obj.vertices.data(), verticesSize, resourceOption));

  auto indicesSize = sizeof(decltype(obj.indices)::value_type) * obj.indices.size();
  m_indexHandle = makeMetalRef(device->getMTLDevice()->newBuffer(obj.indices.data(), indicesSize, resourceOption));

  m_vertexDesc = makeMetalRef(MTL::VertexDescriptor::alloc()->init());

  m_vertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
  m_vertexDesc->attributes()->object(0)->setBufferIndex(VERTEX_BUFFER_INDEX);
  m_vertexDesc->attributes()->object(0)->setOffset(offsetof(Vertex, pos));

  m_vertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
  m_vertexDesc->attributes()->object(1)->setBufferIndex(VERTEX_BUFFER_INDEX);
  m_vertexDesc->attributes()->object(1)->setOffset(offsetof(Vertex, nor));

  m_vertexDesc->attributes()->object(2)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
  m_vertexDesc->attributes()->object(2)->setBufferIndex(VERTEX_BUFFER_INDEX);
  m_vertexDesc->attributes()->object(2)->setOffset(offsetof(Vertex, texCoord));

  m_vertexDesc->layouts()->object(VERTEX_BUFFER_INDEX)->setStride(sizeof(Vertex));

  m_indicesSize = obj.indices.size();
}

MTL::Buffer* MetalVertexBuffer::getVertexHandle() { return m_vertexHandle.get(); }
const MTL::Buffer* MetalVertexBuffer::getVertexHandle() const { return m_vertexHandle.get(); }

MTL::Buffer* MetalVertexBuffer::getIndexHandle() { return m_indexHandle.get(); }
const MTL::Buffer* MetalVertexBuffer::getIndexHandle() const { return m_indexHandle.get(); }

MTL::VertexDescriptor* MetalVertexBuffer::getVertexDescriptor() { return m_vertexDesc.get(); }
const MTL::VertexDescriptor* MetalVertexBuffer::getVertexDescriptor() const { return m_vertexDesc.get(); }

int MetalVertexBuffer::getIndicesSize() const {
  return m_indicesSize;
}
