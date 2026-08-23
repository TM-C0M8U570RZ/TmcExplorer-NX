#include <MainApplication.hpp>
#include "BrowserLayout.hpp"
// Implement all the layout/application functions here

CustomLayout::CustomLayout(MainApplication* app) : Layout::Layout() {
    this->app = app;
    this->greeting = pu::ui::elm::TextBlock::New(10, 10, "Please select a partition or press + to exit");
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
