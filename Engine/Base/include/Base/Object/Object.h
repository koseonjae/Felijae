#pragma once

#include <Base/File/File.h>

#include <glm/glm.hpp>

#include <vector>

namespace larco {
struct Vertex {
  glm::vec3 pos{};
  glm::vec3 nor{};
  glm::vec2 texCoord{};
};

struct Object {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

Object loadObj(const File& file);
} // namespace larco
