#pragma once

class Texture {
 public:
  virtual ~Texture() = default;

  virtual void bind() = 0;
};