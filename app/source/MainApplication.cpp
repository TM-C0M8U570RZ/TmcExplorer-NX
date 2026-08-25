#include <MainApplication.hpp>
#include "BrowserLayout.hpp"
#include <sstream>
// Implement all the layout/application functions here

CustomLayout::CustomLayout(MainApplication* app) : Layout::Layout() {
    this->app = app;
    this->greeting = pu::ui::elm::TextBlock::New(10, 10, "A: Enter partition | X: View space info | -: View info about this software | +: Exit");
    this->partitionSelector = pu::ui::elm::Menu::New(0, 50, 1920, pu::ui::Color(0, 0, 127, 255), pu::ui::Color(127, 127, 255, 255), 128, 5);

    pu::ui::elm::MenuItem::Ref sdmcItem = pu::ui::elm::MenuItem::New("sdmc:");
    sdmcItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/sd.png")));
    sdmcItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    sdmcItem->AddOnKey([this]{
        tmc::ExplorerNX::BrowserLayout::Ref sdmcBrowser = tmc::ExplorerNX::BrowserLayout::New(this->app);
        this->app->LoadLayout(sdmcBrowser);
    });
    sdmcItem->AddOnKey([this]{
        std::filesystem::space_info si = std::filesystem::space("sdmc:/");
        std::stringstream ss;
        ss << "Total capacity: " << tmc::ExplorerNX::beautifyByteCount(si.capacity) << std::endl;
        ss << "Used space: " << tmc::ExplorerNX::beautifyByteCount(si.capacity - si.available) << std::endl;
        ss << "Available space: " << tmc::ExplorerNX::beautifyByteCount(si.available);
        this->app->CreateShowDialog("Summary of sdmc:/", ss.str(), {"OK"}, true);
    }, HidNpadButton_X);
    sdmcItem->AddOnKey([this]{
        this->app->CreateShowDialog("About", "TM_C0M8U570RZ\nElectrify your Life\nC0M8U57 THE TMs!!!!!!\nTmcExplorer-NX was written by Ben from TM_C0M8U570RZ AKA Melsbacksfriend.\nHe may be as controversial as Blawar and Sthetix, but he's still a good software dev.\nBEN FROM TM_C0M8U570RZ IS NOT RESPONSIBLE FOR ANY DAMAGE CAUSED BY MODIFYING EMMC PARTITIONS!", {"OK"}, true);
    }, HidNpadButton_Minus);
    this->partitionSelector->AddItem(sdmcItem);

    pu::ui::elm::MenuItem::Ref prodinfofItem = pu::ui::elm::MenuItem::New("prodinfof:");
    prodinfofItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    prodinfofItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    prodinfofItem->AddOnKey([this]{
        tmc::ExplorerNX::BrowserLayout::Ref prodinfofBrowser = tmc::ExplorerNX::BrowserLayout::New(this->app, "prodinfof:/");
        this->app->LoadLayout(prodinfofBrowser);
    });
    prodinfofItem->AddOnKey([this]{
        std::filesystem::space_info si = std::filesystem::space("prodinfof:/");
        std::stringstream ss;
        ss << "Total capacity: " << tmc::ExplorerNX::beautifyByteCount(si.capacity) << std::endl;
        ss << "Used space: " << tmc::ExplorerNX::beautifyByteCount(si.capacity - si.available) << std::endl;
        ss << "Available space: " << tmc::ExplorerNX::beautifyByteCount(si.available);
        this->app->CreateShowDialog("Summary of prodinfof:/", ss.str(), {"OK"}, true);
    }, HidNpadButton_X);
    prodinfofItem->AddOnKey([this]{
        this->app->CreateShowDialog("About", "TM_C0M8U570RZ\nElectrify your Life\nC0M8U57 THE TMs!!!!!!\nTmcExplorer-NX was written by Ben from TM_C0M8U570RZ AKA Melsbacksfriend.\nHe may be as controversial as Blawar and Sthetix, but he's still a good software dev.\nBEN FROM TM_C0M8U570RZ IS NOT RESPONSIBLE FOR ANY DAMAGE CAUSED BY MODIFYING EMMC PARTITIONS!", {"OK"}, true);
    }, HidNpadButton_Minus);
    this->partitionSelector->AddItem(prodinfofItem);

    pu::ui::elm::MenuItem::Ref safeItem = pu::ui::elm::MenuItem::New("safe:");
    safeItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    safeItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    safeItem->AddOnKey([this]{
        tmc::ExplorerNX::BrowserLayout::Ref safeBrowser = tmc::ExplorerNX::BrowserLayout::New(this->app, "safe:/");
        this->app->LoadLayout(safeBrowser);
    });
    safeItem->AddOnKey([this]{
        std::filesystem::space_info si = std::filesystem::space("safe:/");
        std::stringstream ss;
        ss << "Total capacity: " << tmc::ExplorerNX::beautifyByteCount(si.capacity) << std::endl;
        ss << "Used space: " << tmc::ExplorerNX::beautifyByteCount(si.capacity - si.available) << std::endl;
        ss << "Available space: " << tmc::ExplorerNX::beautifyByteCount(si.available);
        this->app->CreateShowDialog("Summary of safe:/", ss.str(), {"OK"}, true);
    }, HidNpadButton_X);
    safeItem->AddOnKey([this]{
        this->app->CreateShowDialog("About", "TM_C0M8U570RZ\nElectrify your Life\nC0M8U57 THE TMs!!!!!!\nTmcExplorer-NX was written by Ben from TM_C0M8U570RZ AKA Melsbacksfriend.\nHe may be as controversial as Blawar and Sthetix, but he's still a good software dev.\nBEN FROM TM_C0M8U570RZ IS NOT RESPONSIBLE FOR ANY DAMAGE CAUSED BY MODIFYING EMMC PARTITIONS!", {"OK"}, true);
    }, HidNpadButton_Minus);
    this->partitionSelector->AddItem(safeItem);

    pu::ui::elm::MenuItem::Ref systemItem = pu::ui::elm::MenuItem::New("system:");
    systemItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    systemItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    systemItem->AddOnKey([this]{
        tmc::ExplorerNX::BrowserLayout::Ref systemBrowser = tmc::ExplorerNX::BrowserLayout::New(this->app, "system:/");
        this->app->LoadLayout(systemBrowser);
    });
    systemItem->AddOnKey([this]{
        std::filesystem::space_info si = std::filesystem::space("system:/");
        std::stringstream ss;
        ss << "Total capacity: " << tmc::ExplorerNX::beautifyByteCount(si.capacity) << std::endl;
        ss << "Used space: " << tmc::ExplorerNX::beautifyByteCount(si.capacity - si.available) << std::endl;
        ss << "Available space: " << tmc::ExplorerNX::beautifyByteCount(si.available);
        this->app->CreateShowDialog("Summary of system:/", ss.str(), {"OK"}, true);
    }, HidNpadButton_X);
    systemItem->AddOnKey([this]{
        this->app->CreateShowDialog("About", "TM_C0M8U570RZ\nElectrify your Life\nC0M8U57 THE TMs!!!!!!\nTmcExplorer-NX was written by Ben from TM_C0M8U570RZ AKA Melsbacksfriend.\nHe may be as controversial as Blawar and Sthetix, but he's still a good software dev.\nBEN FROM TM_C0M8U570RZ IS NOT RESPONSIBLE FOR ANY DAMAGE CAUSED BY MODIFYING EMMC PARTITIONS!", {"OK"}, true);
    }, HidNpadButton_Minus);
    this->partitionSelector->AddItem(systemItem);

    pu::ui::elm::MenuItem::Ref userItem = pu::ui::elm::MenuItem::New("user:");
    userItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    userItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    userItem->AddOnKey([this]{
        tmc::ExplorerNX::BrowserLayout::Ref userBrowser = tmc::ExplorerNX::BrowserLayout::New(this->app, "user:/");
        this->app->LoadLayout(userBrowser);
    });
    userItem->AddOnKey([this]{
        std::filesystem::space_info si = std::filesystem::space("user:/");
        std::stringstream ss;
        ss << "Total capacity: " << tmc::ExplorerNX::beautifyByteCount(si.capacity) << std::endl;
        ss << "Used space: " << tmc::ExplorerNX::beautifyByteCount(si.capacity - si.available) << std::endl;
        ss << "Available space: " << tmc::ExplorerNX::beautifyByteCount(si.available);
        this->app->CreateShowDialog("Summary of user:/", ss.str(), {"OK"}, true);
    }, HidNpadButton_X);
    userItem->AddOnKey([this]{
        this->app->CreateShowDialog("About", "TM_C0M8U570RZ\nElectrify your Life\nC0M8U57 THE TMs!!!!!!\nTmcExplorer-NX was written by Ben from TM_C0M8U570RZ AKA Melsbacksfriend.\nHe may be as controversial as Blawar and Sthetix, but he's still a good software dev.\nBEN FROM TM_C0M8U570RZ IS NOT RESPONSIBLE FOR ANY DAMAGE CAUSED BY MODIFYING EMMC PARTITIONS!", {"OK"}, true);
    }, HidNpadButton_Minus);
    this->partitionSelector->AddItem(userItem);

    this->Add(this->greeting);
    this->Add(this->partitionSelector);
}

MainApplication::~MainApplication()
{
    tmc::ExplorerNX::unmountEmmcPartition("prodinfof");
    tmc::ExplorerNX::unmountEmmcPartition("safe");
    tmc::ExplorerNX::unmountEmmcPartition("system");
    tmc::ExplorerNX::unmountEmmcPartition("user");
}

void MainApplication::resetLayout()
{
    this->LoadLayout(this->layout);
}

Clipboard MainApplication::getClipboard()
{
    return this->clip;
}

void MainApplication::setClipboard(const Clipboard& clip)
{
    this->clip = clip;
}

bool MainApplication::isClipboardEmpty()
{
    return this->clip.paths.empty();
}

void MainApplication::clearClipboard()
{
    this->clip = {.moving = false, .paths = {}};
}

void MainApplication::OnLoad() {
    tmc::ExplorerNX::mountEmmcPartition("prodinfof");
    tmc::ExplorerNX::mountEmmcPartition("safe");
    tmc::ExplorerNX::mountEmmcPartition("system");
    tmc::ExplorerNX::mountEmmcPartition("user");

    this->clearClipboard();

    this->layout = CustomLayout::New(this);

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(this->layout);
}
