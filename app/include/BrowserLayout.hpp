#pragma once

#include <pu/Plutonium>
#include "Fs.hpp"
#include <sstream>
#include <iomanip>

class MainApplication;

namespace tmc {
namespace ExplorerNX {

class BrowserLayout : public pu::ui::Layout {

private:
    std::filesystem::path currentDirectory;
    MainApplication* app;
    pu::ui::elm::Menu::Ref directoryView;
    pu::ui::elm::TextBlock::Ref controlsRibbon;
    pu::sdl2::TextureHandle::Ref directoryIcon;
    pu::sdl2::TextureHandle::Ref directorySelIcon;
    pu::sdl2::TextureHandle::Ref fileIcon;
    pu::sdl2::TextureHandle::Ref fileSelIcon;

public:
    BrowserLayout(MainApplication* app, const std::filesystem::path& currentDirectory = "sdmc:/");
    PU_SMART_CTOR(BrowserLayout)

};

}
}
