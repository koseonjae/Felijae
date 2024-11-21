#include <Base/Triangle.h>

Object Triangle::load() {
  Object obj{};
  Vertex v{};

  v.pos = glm::vec3(0.0, 1.0, 0.0);
  v.nor = glm::vec3(0.0, 0.0, 1.0);
  v.texCoord = glm::vec2(1.0, 0.0);
  obj.vertices.push_back(v);

  v.pos = glm::vec3(-1.0, -1.0, 0.0);
  v.nor = glm::vec3(0.0, 0.0, 1.0);
  v.texCoord = glm::vec2(0.0, 0.0);
  obj.vertices.push_back(v);

  v.pos = glm::vec3(1.0, -1.0, 0.0);
  v.nor = glm::vec3(0.0, 0.0, 1.0);
  v.texCoord = glm::vec2(0.0, 1.0);
  obj.vertices.push_back(v);

  obj.indices.push_back(0);
  obj.indices.push_back(1);
  obj.indices.push_back(2);

  return obj;
}
