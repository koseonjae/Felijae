#include <Base/File/File.h>
#include <Base/Utility/FileReader.h>

#include <cassert>
#include <filesystem>

std::unordered_map<std::string, std::string> File::m_assetPaths{};

File::File(std::string_view path) : m_path(path.data()) {}

void File::registerPath(const std::string& assetPath, std::string prefix) {
  m_assetPaths[std::move(prefix)] = assetPath + "/";
}

std::string File::read() const {
  auto path = getPath();
  auto str = readFile(path);
  return str;
}

std::string File::getPath() const {
  auto path = m_path;
  for (const auto& [prefix, prefixPath] : m_assetPaths) {
    if (path.starts_with(prefix)) {
      path.replace(0, prefix.size(), prefixPath);
      break;
    }
  }

  auto exist = std::filesystem::exists(path);
  assert(exist && "File is not found");

  return path;
}