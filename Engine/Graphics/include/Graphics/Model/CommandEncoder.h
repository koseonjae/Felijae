#pragma once

#include <vector>
#include <memory>

namespace goala {
class Pipeline;
class Fence;

class CommandEncoder {
public:
  virtual ~CommandEncoder() = default;

  virtual void encode(Pipeline* pipeline) = 0;

  virtual void updateDependency(const std::vector<std::shared_ptr<Fence>>& waitFences,
                                const std::vector<std::shared_ptr<Fence>>& signalFences) = 0;
};
} // namespace goala
