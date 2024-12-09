#pragma once

#include <Base/Object/Object.h>

#include <memory>

namespace goala {

struct VertexBufferDescription {
  Object object;
};

class Buffer {
 public:
  virtual ~Buffer() = default;
};

} // namespace goala
