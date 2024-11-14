#include <Utility/FileReader.h>
#include <Metal/triangle_types.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <SDL.h>

#include <iostream>

namespace {
const std::vector<int> viewport = {640, 480};

const std::vector<AAPLVertex> triangleVertices = {
    // 2D positions,    RGBA colors
    {{0, 1}, {1, 0, 0, 1}},
    {{-1, -1}, {0, 1, 0, 1}},
    {{1, -1}, {0, 0, 1, 1}},
};

template<typename T>
class MetalPtr {
 public:
  explicit MetalPtr(T* ptr) : m_ptr(ptr) { m_ptr->retain(); }
  ~MetalPtr() { m_ptr->release(); }
  MetalPtr(const MetalPtr<T>& rhs) = delete;
  MetalPtr(MetalPtr<T>&& rhs) = delete;
  MetalPtr& operator=(const MetalPtr<T>& rhs) = delete;
  MetalPtr& operator=(MetalPtr<T>&& rhs) = delete;
  auto operator->() { return m_ptr; }
  auto operator->() const { return m_ptr; }
  explicit operator bool() const { return m_ptr; }
  [[nodiscard]] T* get() const { return m_ptr; }
 private:
  T* m_ptr = nullptr;
};

inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

int main(int argc, char** argv) {
  NS::Error* err = nil;

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("SDL Metal", -1, -1, viewport[0], viewport[1], SDL_WINDOW_ALLOW_HIGHDPI);

  auto device = MetalPtr(MTL::CreateSystemDefaultDevice());

  SDL_MetalView view = SDL_Metal_CreateView(window);
  auto layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(view)); // swapchain
  layer->setDevice(device.get());
  layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

  auto verticesSize = sizeof(AAPLVertex) * triangleVertices.size();
  auto vertexBuffer = MetalPtr(device->newBuffer(triangleVertices.data(), verticesSize, MTL::ResourceCPUCacheModeDefaultCache));

  auto vertexDesc = MetalPtr(MTL::VertexDescriptor::alloc()->init());
  vertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
  vertexDesc->attributes()->object(0)->setBufferIndex(AAPLVertexInputIndex::AAPLVertexInputIndexVertices);
  vertexDesc->attributes()->object(0)->setOffset(offsetof(AAPLVertex, position));
  vertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat4);
  vertexDesc->attributes()->object(1)->setBufferIndex(AAPLVertexInputIndex::AAPLVertexInputIndexVertices);
  vertexDesc->attributes()->object(1)->setOffset(offsetof(AAPLVertex, color));
  vertexDesc->layouts()->object(0)->setStride(sizeof(AAPLVertex));

  auto metalShaders = readFileToString("../asset/shader/triangle.metal");
  auto library = MetalPtr(device->newLibrary(getNSString(metalShaders), nullptr, &err));
  assert(library && "Failed to create library");

  auto vertexFuncName = NS::String::string("vertexShader", NS::ASCIIStringEncoding);
  auto vertexFunc = MetalPtr(library->newFunction(vertexFuncName));

  auto fragmentFuncName = NS::String::string("fragmentShader", NS::ASCIIStringEncoding);
  auto fragmentFunc = MetalPtr(library->newFunction(fragmentFuncName));

  auto pipelineDesc = MetalPtr(MTL::RenderPipelineDescriptor::alloc()->init());
  pipelineDesc->setVertexFunction(vertexFunc.get());
  pipelineDesc->setFragmentFunction(fragmentFunc.get());
  pipelineDesc->setVertexDescriptor(vertexDesc.get());

  auto colorAttachmentDesc = pipelineDesc->colorAttachments()->object(0);
  colorAttachmentDesc->setPixelFormat(layer->pixelFormat());

  auto pipeline = MetalPtr(device->newRenderPipelineState(pipelineDesc.get(), &err));
  assert(pipeline && "Failed to create pipeline");

  auto queue = MetalPtr(device->newCommandQueue());

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }
      }
    }

    auto drawable = MetalPtr(layer->nextDrawable());

    auto pass = MetalPtr(MTL::RenderPassDescriptor::renderPassDescriptor());

    auto colorAttachment = pass->colorAttachments()->object(0);
    colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
    colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    colorAttachment->setTexture(drawable->texture());

    auto buffer = MetalPtr(queue->commandBuffer());

    // encoding
    auto encoder = MetalPtr(buffer->renderCommandEncoder(pass.get()));
    encoder->setRenderPipelineState(pipeline.get());
    encoder->setVertexBuffer(vertexBuffer.get(), 0, AAPLVertexInputIndexVertices);
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, 0ul, 3ul);
    encoder->endEncoding();

    buffer->presentDrawable(drawable.get());
    buffer->commit();
  }

  SDL_Metal_DestroyView(view);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}