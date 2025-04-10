#pragma once

#include <vector>
#include <functional>

namespace larco {
class Pipeline;
class Fence;

struct CommandEncoderDescription {};

class CommandEncoder {
public:
  virtual ~CommandEncoder() = default;

  virtual void endEncoding() = 0;

  virtual void encodeDraw(Pipeline* pipeline) = 0;
};
} // namespace larco
