#pragma once

#include <switch.h>
#include <filesystem>
#include <string>

namespace tmc {
namespace ExplorerNX {

bool mountEmmcPartition(const std::string& emmcPartition);

void unmountEmmcPartition(const std::string& emmcPartition);

}
}
