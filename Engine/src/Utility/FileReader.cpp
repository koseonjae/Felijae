#include <Utility/FileReader.h>

#include <iostream>
#include <fstream>

std::string readFile(const std::filesystem::path& filePath) {
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
