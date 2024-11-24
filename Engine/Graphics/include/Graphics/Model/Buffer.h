#pragma once

namespace goala {

class Object;

class Buffer {
 public:
  virtual ~Buffer() = default;

  virtual void initialize(const Object& obj) = 0;

  virtual void release() = 0;

  virtual void bind() = 0;

  virtual void draw() = 0;
};

} // namespace goala
