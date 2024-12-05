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

  virtual void bind() = 0;

  virtual void draw() = 0;
};

} // namespace goala
