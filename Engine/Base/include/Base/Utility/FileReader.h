#pragma once

#include <filesystem>
#include <string>

namespace goala {

std::string readFile(const std::filesystem::path& filePath);

} // namespace goala
