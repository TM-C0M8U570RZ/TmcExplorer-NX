#include "Fs.hpp"


namespace tmc {
namespace ExplorerNX {

const char* ngFilenames[] = {"aux", "com1", "com2", "com3", "com4", "com5", "com6", "com7", "com8", "com9", "con", "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9", "nul", "prn"};

const char* ngChars = "<>:\"/\\|?*";

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

void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination, u64* fileCountProgress, u64* bytesProgress)
{
    if (std::filesystem::is_regular_file(source))
    {
        u64 fileSize = std::filesystem::file_size(source);
        u64 bytesAlreadyCopied = 0;
        bool shouldConcatenate = (fileSize > FAT32_SIZE_LIMIT);
        std::ifstream in(source, std::ios::binary);
        std::vector<char> data(1024 * 1024, 0);
        if (shouldConcatenate)
        {
            fsdevCreateFile(destination.string().c_str(), 0, FsCreateOption_BigFile);
        }
        std::ofstream out(destination, std::ios::binary);
        while(in.peek() != -1)
        {
            if ((fileSize - bytesAlreadyCopied) < (1024 * 1024))
                data.resize(fileSize - bytesAlreadyCopied);
            in.read(data.data(), data.size());
            out.write(data.data(), data.size());
            bytesAlreadyCopied += data.size();
            (*bytesProgress) += data.size();
        }
        (*fileCountProgress)++;
    }
    else if (std::filesystem::is_directory(source))
    {
        std::filesystem::create_directories(destination);
        for (auto& p: std::filesystem::directory_iterator(source))
        {
            copyFiles(p.path(), destination / p.path().filename(), fileCountProgress, bytesProgress);
        }
    }
}

bool getFilenameFromKeyboard(char* outStr, std::size_t maxLen)
{
    SwkbdConfig kbd;
    Result rc = 0;

    rc = swkbdCreate(&kbd, 0);
    if (R_FAILED(rc)) return false;
    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetGuideText(&kbd, "filename");
    swkbdConfigSetStringLenMax(&kbd, maxLen - 1);
    rc = swkbdShow(&kbd, outStr, maxLen);
    swkbdClose(&kbd);

    if (R_SUCCEEDED(rc))
    {
        std::filesystem::path newPath = std::filesystem::path(std::string(outStr));
        std::string lowercaseStem;

        for (u16 i = 0; i < newPath.stem().string().size(); i++)
        {
            lowercaseStem += (char)(tolower(newPath.stem().string()[i]));
        }

        for (u8 i = 0; i < 22; i++)
        {
            std::string currentNgName = ngFilenames[i];
            if (lowercaseStem == currentNgName) return false;
        }

        for (u8 i = 0; i < strlen(ngChars); i++)
        {
            std::size_t ngCharPos = newPath.string().find(ngChars[i]);
            if (ngCharPos != std::string::npos) return false;
        }

        if (newPath.string().back() == ' ' || newPath.string().back() == '.') return false;
        if (newPath.string().empty()) return false;
        return true;
    }
    return false;

}

u64 enumerateBytes(const std::filesystem::path& p)
{
    if (std::filesystem::is_regular_file(p)) return std::filesystem::file_size(p);
    u64 bytes = 0;
    for (auto& e: std::filesystem::recursive_directory_iterator(p))
    {
        if (std::filesystem::is_regular_file(e.path())) bytes += std::filesystem::file_size(e.path());
    }
    return bytes;
}

u64 enumerateFiles(const std::filesystem::path& p)
{
    if (std::filesystem::is_regular_file(p)) return 1;
    u64 files = 0;
    for (auto& e: std::filesystem::recursive_directory_iterator(p))
    {
        if (std::filesystem::is_regular_file(e.path())) files++;
    }
    return files;
}

std::string beautifyByteCount(u64 bytes)
{
    if (bytes < 1024) return std::to_string(bytes) + "B";
    const char* units[] = {"KiB", "MiB", "GiB", "TiB"};
    for (u8 i = 0; i < 4; i++)
    {
        bytes >>= 10;
        if (bytes < 1024) return std::to_string(bytes) + std::string(units[i]);
    }
    // this should never happen on an SD card or eMMC as this requires storage capacities measured in PiB, but I added this to silence compile-time warnings.
    return std::to_string(bytes) + "TiB";
}

void deleteFiles(const std::filesystem::path& p, u64* fileCountProgress, u64* bytesProgress)
{
    if (std::filesystem::is_regular_file(p))
    {
        u64 fileSize = std::filesystem::file_size(p);
        std::filesystem::remove(p);
        (*bytesProgress) += fileSize;
        (*fileCountProgress)++;
    }
    else if (std::filesystem::is_directory(p))
    {
        for (auto& e: std::filesystem::directory_iterator(p))
        {
            deleteFiles(e.path(), fileCountProgress, bytesProgress);
        }
        // the directory should be empty after the recursions.
        std::filesystem::remove(p);
    }
}

std::string extractPartition(const std::filesystem::path& p)
{
    std::size_t found = p.string().find(':');
    if (found != std::string::npos)
    {
        return p.string().substr(0, found + 1);
    }
    return p.string();
}

void moveFiles(const std::filesystem::path& source, const std::filesystem::path& destination, u64* fileCountProgress, u64* bytesProgress)
{
    if (extractPartition(source) == extractPartition(destination))
    {
        if (std::filesystem::is_regular_file(source))
        {
            std::filesystem::rename(source, destination);
            (*fileCountProgress)++;
            (*bytesProgress) += std::filesystem::file_size(destination);
        }
        else
        {
            for (auto& p: std::filesystem::directory_iterator(source))
            {
                std::filesystem::create_directories(destination);
                moveFiles(p.path(), destination / p.path().filename(), fileCountProgress, bytesProgress);
            }
            std::filesystem::remove(source);
        }
    }
    else
    {
        u64 dummy1 = 0;
        u64 dummy2 = 0;
        copyFiles(source, destination, fileCountProgress, bytesProgress);
        deleteFiles(source, &dummy1, &dummy2);
    }
}

}
}
