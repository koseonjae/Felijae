#include <Base/Utility/FileReader.h>

#include <fstream>
#include <iostream>

namespace larco {
std::string larco::readFile(const std::filesystem::path& filePath) {
  std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
  if (!fileStream) {
    throw std::runtime_error("Could not open file: " + filePath.string());
  }

  std::string content;
  fileStream.seekg(0, std::ios::end);
  content.resize(fileStream.tellg());
  fileStream.seekg(0, std::ios::beg);
  fileStream.read(&content[0], content.size());
  fileStream.close();

  return content;
}
} // namespace larco
