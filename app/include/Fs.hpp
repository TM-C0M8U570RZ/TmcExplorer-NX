#pragma once

#include <switch.h>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cstring>
#include <cctype>

namespace tmc {
namespace ExplorerNX {

constexpr u64 FAT32_SIZE_LIMIT = u64(std::numeric_limits<u32>::max());

bool mountEmmcPartition(const std::string& emmcPartition);

void unmountEmmcPartition(const std::string& emmcPartition);

void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination, u64* fileCountProgress, u64* bytesProgress);

bool getFilenameFromKeyboard(char* outStr, std::size_t maxLen);

}
}
