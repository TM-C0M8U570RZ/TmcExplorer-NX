#include "Fs.hpp"


namespace tmc {
namespace ExplorerNX {

bool mountEmmcPartition(const std::string& emmcPartition)
{
    Result rc = 0;
    FsFileSystem fs;
    FsBisPartitionId id = FsBisPartitionId_CalibrationFile;
    if (emmcPartition == "prodinfof")
    {

    }
    else if (emmcPartition == "safe")
    {
        id = FsBisPartitionId_SafeMode;
    }
    else if (emmcPartition == "system")
    {
        id = FsBisPartitionId_System;
    }
    else if (emmcPartition == "user")
    {
        id = FsBisPartitionId_User;
    }
    else return false;

    rc = fsOpenBisFileSystem(&fs, id, "");
    if (R_FAILED(rc)) return false;

    int devId = fsdevMountDevice(emmcPartition.c_str(), fs);
    if (devId == -1)
    {
        fsFsClose(&fs);
        return false;
    }
    return true;
}

void unmountEmmcPartition(const std::string& emmcPartition)
{
    fsdevUnmountDevice(emmcPartition.c_str());
}

}
}
