#pragma once

namespace goala {

class Pipeline;

class CommandEncoder {
 public:
  virtual ~CommandEncoder() = default;

  virtual void encode(Pipeline* pipeline) = 0;

  virtual void endEncoding() = 0;
};

} // namespace goala
