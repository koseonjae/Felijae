#include <Metal/Metal.hpp>
#include <Base/Utility/FileReader.h>
#include <Base/File/File.h>

#include <Metal/MTLLibrary.hpp>

#include <iostream>
#include <vector>

using namespace goala;

namespace {
inline NS::String* getNSString(std::string_view str) {
  return NS::String::string(str.data(), NS::ASCIIStringEncoding);
}
} // namespace

int main() {
  goala::File::registerPath(DEMO_DIR + std::string("/asset"), "asset://");

  // Metal 장치 초기화
  auto device = MTL::CreateSystemDefaultDevice();
  if (!device) {
    std::cerr << "Metal 지원되지 않음." << std::endl;
    return -1;
  }

  auto commandQueue = device->newCommandQueue();

  auto computeShaderCode = File("asset://shader/compute_example.msl").read();

  // Shader 컴파일
  NS::Error* err = nil;
  auto library = device->newLibrary(getNSString(computeShaderCode), nullptr, &err);
  if (err)
    std::cout << "Filed to create library: " << err->localizedDescription()->utf8String() << std::endl;

  auto function = library->newFunction(NS::String::string("compute_shader", NS::ASCIIStringEncoding));
  if (err)
    std::cout << "Filed to create function: " << err->localizedDescription()->utf8String() << std::endl;

  auto pipelineState = device->newComputePipelineState(function, &err);
  if (err)
    std::cout << "Filed to create pipeline: " << err->localizedDescription()->utf8String() << std::endl;

  // 행렬 크기 정의
  const uint rows = 4;
  const uint cols = 4;
  const size_t totalElements = rows * cols;

  // 입력 버퍼 생성
  std::vector<float> inputData(totalElements);
  for (size_t i = 0; i < totalElements; ++i) {
    inputData[i] = static_cast<float>(i);
  }

  auto inputBuffer = device->newBuffer(inputData.data(), inputData.size() * sizeof(float), MTL::ResourceStorageModeShared);

  // 출력 텍스처 생성
  auto textureDescriptor = MTL::TextureDescriptor::texture2DDescriptor(
    MTL::PixelFormatRGBA32Float, cols, rows, false);
  textureDescriptor->setUsage(MTL::TextureUsageShaderWrite);
  auto outputTexture = device->newTexture(textureDescriptor);

  // 커맨드 버퍼와 인코더 생성
  auto commandBuffer = commandQueue->commandBuffer();
  auto computeEncoder = commandBuffer->computeCommandEncoder();

  // Compute Shader 설정
  computeEncoder->setComputePipelineState(pipelineState);
  computeEncoder->setBuffer(inputBuffer, 0, 0);
  computeEncoder->setTexture(outputTexture, 0);

  computeEncoder->setBytes(&rows, sizeof(rows), 1);
  computeEncoder->setBytes(&cols, sizeof(cols), 2);

  // 스레드 그룹 설정
  MTL::Size threadGroupSize(8, 8, 1);
  MTL::Size threadGroups((cols + 7) / 8, (rows + 7) / 8, 1);
  computeEncoder->dispatchThreadgroups(threadGroups, threadGroupSize);

  computeEncoder->endEncoding();
  commandBuffer->commit();
  commandBuffer->waitUntilCompleted();

  // 결과 확인
  auto outputBuffer = std::make_unique<float[]>(totalElements * 4); // RGBA 포맷
  MTL::Region region = MTL::Region::Make2D(0, 0, cols, rows);
  outputTexture->getBytes(outputBuffer.get(), cols * 4 * sizeof(float), region, 0);

  std::cout << "결과 텍스처 값:" << std::endl;
  for (uint y = 0; y < rows; ++y) {
    for (uint x = 0; x < cols; ++x) {
      size_t index = (y * cols + x) * 4; // RGBA 채널
      std::cout << outputBuffer[index] << " ";
    }
    std::cout << std::endl;
  }

  // 메모리 해제
  inputBuffer->release();
  outputTexture->release();
  computeEncoder->release();
  commandBuffer->release();
  pipelineState->release();
  function->release();
  library->release();
  commandQueue->release();
  device->release();

  return 0;
}
