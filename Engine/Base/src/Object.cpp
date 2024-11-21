#include <Base/Object.h>
#include <tiny_obj_loader.h>

Object loadObj(std::string_view path) {
  tinyobj::ObjReader reader;
  tinyobj::ObjReaderConfig config{};
  bool success = reader.ParseFromFile(path.data(), config);
  assert(success && "Failed to parse obj.");

  auto& attributes = reader.GetAttrib();
  auto& shapes = reader.GetShapes();
  auto& indices = shapes[0].mesh.indices;

  Object result;

  int newIdx = 0;
  std::map<std::tuple<float, float, float>, int> newIdxMap;
  for (int i = 0; i < indices.size(); ++i) {
    std::tuple idx{indices[i].vertex_index, indices[i].normal_index, indices[i].texcoord_index};
    auto found = newIdxMap.find(idx);
    if (found == newIdxMap.end()) {
      auto vIdx = 3 * indices[i].vertex_index;
      auto nIdx = 3 * indices[i].normal_index;
      auto uvIdx = 2 * indices[i].texcoord_index;
      result.vertices.push_back(
          {
              {attributes.vertices[vIdx], attributes.vertices[vIdx + 1], attributes.vertices[vIdx + 2]},
              {attributes.normals[nIdx], attributes.normals[nIdx + 1], attributes.normals[nIdx + 2]},
              {attributes.texcoords[uvIdx], attributes.texcoords[uvIdx + 1]},
          });
      newIdxMap.insert({std::move(idx), newIdx});
      result.indices.push_back(newIdx);
      ++newIdx;
    } else {
      result.indices.push_back(found->second);
    }
  }

  return result;
}
