#pragma once

#include <string>

namespace larco {

class Node : public std::enable_shared_from_this<Node> {
 public:
  virtual ~Node() = default;
};

} // namespace larco
