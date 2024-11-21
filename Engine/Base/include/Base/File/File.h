#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace goala {

class File {
 public:
  static void registerPath(const std::string& assetPath, std::string prefix);

  explicit File(std::string_view path);

  std::string read() const;

  std::string getPath() const;

 private:
  static std::unordered_map<std::string, std::string> m_assetPaths;
  std::string m_path;
};

} // namespace goala
