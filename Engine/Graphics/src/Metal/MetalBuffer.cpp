#include <Base/Object/Object.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/MTLVertexDescriptor.hpp>

using namespace goala;

void MetalBuffer::initialize(const Object& obj) {
  assert(false && "todo: implement");
}

void MetalBuffer::release() {
  assert(false && "todo: implement");
}

void MetalBuffer::bind() {
  assert(false && "todo: implement");
}

void MetalBuffer::draw() {
  assert(false && "draw에서 encoding을 해야하나?");
}

MetalBuffer::MetalBuffer(MetalDevice* device, const Object& obj) {
  auto resourceOption = MTL::ResourceCPUCacheModeDefaultCache;

  auto verticesSize =
    sizeof(decltype(obj.vertices)::value_type) * obj.vertices.size();
  m_vertexHandle = MetalRef(device->getMTLDevice()->newBuffer(
    (void*) obj.vertices.data(), verticesSize, resourceOption));

  auto indicesSize =
    sizeof(decltype(obj.indices)::value_type) * obj.indices.size();
  m_indexHandle = MetalRef(device->getMTLDevice()->newBuffer(
    obj.indices.data(), indicesSize, resourceOption));

  m_vertexDesc = MetalRef(MTL::VertexDescriptor::alloc()->init());

  m_vertexDesc->attributes()->object(0)->setFormat(
    MTL::VertexFormat::VertexFormatFloat3);
  m_vertexDesc->attributes()->object(0)->setBufferIndex(0);
  m_vertexDesc->attributes()->object(0)->setOffset(offsetof(Vertex, pos));

  m_vertexDesc->attributes()->object(1)->setFormat(
    MTL::VertexFormat::VertexFormatFloat3);
  m_vertexDesc->attributes()->object(1)->setBufferIndex(0);
  m_vertexDesc->attributes()->object(1)->setOffset(offsetof(Vertex, nor));

  m_vertexDesc->attributes()->object(2)->setFormat(
    MTL::VertexFormat::VertexFormatFloat2);
  m_vertexDesc->attributes()->object(2)->setBufferIndex(0);
  m_vertexDesc->attributes()->object(2)->setOffset(offsetof(Vertex, texCoord));

  m_vertexDesc->layouts()->object(0)->setStride(sizeof(Vertex));

  m_indicesSize = obj.indices.size();
}

MTL::Buffer* MetalBuffer::getVertexHandle() const {
  return m_vertexHandle.get();
}

MTL::Buffer* MetalBuffer::getIndexHandle() const { return m_indexHandle.get(); }

MTL::VertexDescriptor* MetalBuffer::getVertexDescriptor() const {
  return m_vertexDesc.get();
}

int MetalBuffer::getIndicesSize() const {
  return m_indicesSize;
}
