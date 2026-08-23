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
        return true;
    }
    return false;

}

}
}
