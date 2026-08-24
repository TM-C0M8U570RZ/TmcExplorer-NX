#pragma once

#include <switch.h>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iomanip>

namespace tmc {
namespace ExplorerNX {

constexpr u64 FAT32_SIZE_LIMIT = u64(std::numeric_limits<u32>::max());

bool mountEmmcPartition(const std::string& emmcPartition);

void unmountEmmcPartition(const std::string& emmcPartition);

void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination, u64* fileCountProgress, u64* bytesProgress);

bool getFilenameFromKeyboard(char* outStr, std::size_t maxLen);

u64 enumerateBytes(const std::filesystem::path& p);

u64 enumerateFiles(const std::filesystem::path& p);

std::string beautifyByteCount(u64 bytes);

void deleteFiles(const std::filesystem::path& p, u64* fileCountProgress, u64* bytesProgress);

std::string extractPartition(const std::filesystem::path& p);

void moveFiles(const std::filesystem::path& source, const std::filesystem::path& destination, u64* fileCountProgress, u64* bytesProgress);

}
}
