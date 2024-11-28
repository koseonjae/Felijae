#pragma once

#include <vector>
#include <functional>

namespace goala {
class Pipeline;
class Fence;

struct CommandEncoderDescription {};

class CommandEncoder {
public:
  virtual ~CommandEncoder() = default;

  virtual void endEncoding() = 0;

  virtual void encodeDraw(Pipeline* pipeline) = 0;

  virtual void updateDependency(const std::vector<std::shared_ptr<Fence>>& waitFences,
                                const std::vector<std::shared_ptr<Fence>>& signalFences) = 0;
};
} // namespace goala
