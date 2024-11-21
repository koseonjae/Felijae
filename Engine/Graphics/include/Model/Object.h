#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 nor;
  glm::vec2 texCoord;
};

struct Object {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

Object loadObj(std::string_view path);
