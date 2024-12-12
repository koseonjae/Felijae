#pragma once

namespace goala {
class ComputePipeline;
struct ComputeCommandEncoderDescription {};

class ComputeCommandEncoder {
public:
  virtual ~ComputeCommandEncoder() = default;

  virtual void encodeCompute(ComputePipeline* pipeline) = 0;

  virtual void endEncoding() = 0;
};
} // namespace goala
