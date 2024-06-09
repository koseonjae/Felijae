#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 pos;
};

struct Object {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};
