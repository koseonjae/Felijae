#pragma once

#include <filesystem>
#include <string>

namespace larco {
std::string readFile(const std::filesystem::path& filePath);
} // namespace larco
