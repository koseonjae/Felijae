#include "triangle_types.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <SDL.h>

#include <iostream>
#include <fstream>

namespace {

#include "triangle_metallib.h"

const AAPLVertex triangleVertices[] = {
    // 2D positions,    RGBA colors
    {{250, -250}, {1, 0, 0, 1}},
    {{-250, -250}, {0, 1, 0, 1}},
    {{0, 250}, {0, 0, 1, 1}},
};

const vector_uint2 viewport = {640, 480};

std::string readFileToString(const std::filesystem::path& filePath) {
  std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
  if (!fileStream) {
    throw std::runtime_error("Could not open file: " + filePath.string());
  }

  std::string content;
  fileStream.seekg(0, std::ios::end);
  content.resize(fileStream.tellg());
  fileStream.seekg(0, std::ios::beg);
  fileStream.read(&content[0], content.size());
  fileStream.close();

  return content;
}

}

template<typename T>
class MetalPtr {
 public:
  MetalPtr(T *ptr) : m_ptr(ptr) { m_ptr->retain(); }
  ~MetalPtr() { m_ptr->release(); }
  MetalPtr(const MetalPtr<T>& rhs) = delete;
  MetalPtr(MetalPtr<T>&& rhs) = delete;
  MetalPtr& operator=(const MetalPtr<T>& rhs) = delete;
  MetalPtr& operator=(MetalPtr<T>&& rhs) = delete;

  auto operator->() { return m_ptr; }
  auto operator->() const { return m_ptr; }
  explicit operator bool() const { return m_ptr; }
  auto get() const { return m_ptr; }

 private:
  T *m_ptr = nullptr;
};

int main(int argc, char **argv) {
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("SDL Metal", -1, -1, viewport[0], viewport[1], SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

  NS::Error *err;

  auto swapchain = (CA::MetalLayer *) SDL_RenderGetMetalLayer(renderer);
  auto device = swapchain->device();

  auto name = device->name();
  std::cerr << "device name: " << name->utf8String() << std::endl;

  auto library_data = dispatch_data_create(
      &triangle_metallib[0], triangle_metallib_len,
      dispatch_get_main_queue(),
      ^{});

  auto shaderSource = readFileToString("../triangle.metal");

  auto library = MetalPtr(device->newLibrary(library_data, &err));

  if (!library) {
    std::cerr << "Failed to create library" << std::endl;
    std::exit(-1);
  }

  auto vertex_function_name = NS::String::string("vertexShader", NS::ASCIIStringEncoding);
  auto vertex_function = MetalPtr(library->newFunction(vertex_function_name));

  auto fragment_function_name = NS::String::string("fragmentShader", NS::ASCIIStringEncoding);
  auto fragment_function = MetalPtr(library->newFunction(fragment_function_name));

  auto pipeline_descriptor = MetalPtr(MTL::RenderPipelineDescriptor::alloc()->init());
  pipeline_descriptor->setVertexFunction(vertex_function.get());
  pipeline_descriptor->setFragmentFunction(fragment_function.get());

  auto color_attachment_descriptor = pipeline_descriptor->colorAttachments()->object(0);
  color_attachment_descriptor->setPixelFormat(swapchain->pixelFormat());

  auto pipeline = MetalPtr(device->newRenderPipelineState(pipeline_descriptor.get(), &err));

  if (!pipeline) {
    std::cerr << "Failed to create pipeline" << std::endl;
    std::exit(-1);
  }

  auto queue = MetalPtr(device->newCommandQueue());

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
        }
          break;
      }
    }

    auto drawable = swapchain->nextDrawable();

    auto pass = MetalPtr(MTL::RenderPassDescriptor::renderPassDescriptor());

    auto color_attachment = pass->colorAttachments()->object(0);
    color_attachment->setLoadAction(MTL::LoadAction::LoadActionClear);
    color_attachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    color_attachment->setTexture(drawable->texture());

    //
    auto buffer = MetalPtr(queue->commandBuffer());

    //
    auto encoder = MetalPtr(buffer->renderCommandEncoder(pass.get()));

    encoder->setViewport(MTL::Viewport{
        0.0f, 0.0f,
        (double) viewport[0], (double) viewport[1],
        0.0f, 1.0f
    });

    encoder->setRenderPipelineState(pipeline.get());

    encoder->setVertexBytes(&triangleVertices[0], sizeof(triangleVertices), AAPLVertexInputIndexVertices);
    encoder->setVertexBytes(&viewport, sizeof(viewport), AAPLVertexInputIndexViewportSize);

    NS::UInteger vertex_start = 0, vertex_count = 3;
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, vertex_start, vertex_count);

    encoder->endEncoding();

    buffer->presentDrawable(drawable);
    buffer->commit();

    drawable->release();
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}