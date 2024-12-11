#pragma once

#include <Engine/Model/Camera.h>
#include <Engine/Model/Light.h>
#include <Engine/Model/Node.h>

#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

namespace goala {
class Model;

class Scene final {
public:
  void addModel(std::shared_ptr<Model> model);

  const std::vector<std::shared_ptr<Model>>& getModels() const;

  std::vector<std::shared_ptr<Model>>& getModels();

  template <typename T>
  void setNode(std::shared_ptr<T> node) {
    assert(m_nodes.find(typeid(T)) == m_nodes.end() && "Duplicate node type is not supported");
    m_nodes.insert({typeid(T), std::move(node)});
  }

  template <typename T>
  T* getNode() {
    auto found = m_nodes.find(typeid(T));
    if (found == m_nodes.end())
      return nullptr;
    T* ptr = dynamic_cast<T*>(found->second.get());
    assert(ptr && "Failed to cast node type");
    return ptr;
  }

  template <typename T>
  const T* getNode() const {
    auto found = m_nodes.find(typeid(T));
    if (found == m_nodes.end())
      return nullptr;
    const T* ptr = reinterpret_cast<T*>(found->second.get());
    assert(ptr && "Failed to cast node type");
    return ptr;
  }

private:
  std::unordered_map<std::type_index, std::shared_ptr<Node>> m_nodes;
  std::vector<std::shared_ptr<Model>> m_models;
};
} // namespace goala
