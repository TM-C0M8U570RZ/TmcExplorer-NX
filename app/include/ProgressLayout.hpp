#pragma once

#include <pu/Plutonium>
#include "Fs.hpp"
#include <iomanip>
#include <sstream>
#include <pthread.h>
#include <thread>

class MainApplication;

namespace tmc {
namespace ExplorerNX {

enum class ProgressMode : u8 {
    MOVE = 0,
    COPY,
    DELETE
};

struct ThreadContext {
    ProgressMode pm;
    std::filesystem::path currentDirectory;
    std::vector<std::filesystem::path> paths;
    u64* fileCountProgress;
    u64* bytesProgress;
};

class ProgressLayout : public pu::ui::Layout {
private:
    MainApplication* app;
    pu::ui::elm::ProgressBar::Ref progress;
    pu::ui::elm::TextBlock::Ref text;
    pu::ui::elm::TextBlock::Ref progressText;
    pu::ui::elm::Rectangle::Ref background;
    std::filesystem::path currentDirectory;
    std::vector<std::filesystem::path> deleteList;
    u64 fileCountProgress;
    u64 fileCountTotal;
    u64 bytesProgress;
    u64 bytesTotal;
    ProgressMode pm;
    ThreadContext* ctx;
    Thread workerThread;

public:
    ProgressLayout(MainApplication* app, const std::filesystem::path& currentDirectory, const std::vector<std::filesystem::path> deleteList = {});
    PU_SMART_CTOR(ProgressLayout);
    ~ProgressLayout();
};

}
}
