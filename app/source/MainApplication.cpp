#include <MainApplication.hpp>

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
    this->partitionSelector->AddItem(sdmcItem);

    pu::ui::elm::MenuItem::Ref prodinfofItem = pu::ui::elm::MenuItem::New("prodinfof:");
    prodinfofItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    prodinfofItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    this->partitionSelector->AddItem(prodinfofItem);

    pu::ui::elm::MenuItem::Ref safeItem = pu::ui::elm::MenuItem::New("safe:");
    safeItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    safeItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    this->partitionSelector->AddItem(safeItem);

    pu::ui::elm::MenuItem::Ref systemItem = pu::ui::elm::MenuItem::New("system:");
    systemItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    systemItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
    this->partitionSelector->AddItem(systemItem);

    pu::ui::elm::MenuItem::Ref userItem = pu::ui::elm::MenuItem::New("user:");
    userItem->SetIcon(pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/emmc.png")));
    userItem->AddOnKey([this]{
        this->app->Close();
    }, HidNpadButton_Plus);
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

void MainApplication::OnLoad() {
    tmc::ExplorerNX::mountEmmcPartition("prodinfof");
    tmc::ExplorerNX::mountEmmcPartition("safe");
    tmc::ExplorerNX::mountEmmcPartition("system");
    tmc::ExplorerNX::mountEmmcPartition("user");

    this->layout = CustomLayout::New(this);

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(this->layout);

    // Set a function when input is caught. This input handling will be the first one to be handled (before Layout or any Elements)
    // Using a lambda function here to simplify things
    // You can use member functions via std::bind() C++ wrapper
    this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos) {
         // If X is pressed, start with our dialog questions!
        if(keys_down & HidNpadButton_X) {
            int opt = this->CreateShowDialog("Question", "Do you like apples?", { "Yes!", "No...", "Cancel" }, true); // (using latest option as cancel option)
            // -1 and -2 are similar, but if the user cancels manually -1 is set, other types or cancel should be -2.
            if((opt == -1) || (opt == -2))  {
                this->CreateShowDialog("Cancel", "Last question was canceled.", { "Ok" }, true); // If we will ignore the option, it doesn't matter if this is true or false
            }
            else {
                // Otherwise, opt will be the index of the options we passed to the dialog
                switch(opt) {
                    // "Yes!" was selected
                    case 0:  {
                        this->CreateShowDialog("Answer", "Really? I like apples too!", { "Ok" }, true); // Same here ^
                        break;
                    }
                    // "No..." was selected
                    case 1: {
                        this->CreateShowDialog("Answer", "Oh, bad news then...", { "Ok" }, true); // And here ^
                        break;
                    }
                }
            }
        }
        // If + is pressed, exit application
        else if(keys_down & HidNpadButton_Plus) {
            this->Close();
        }
    });
}
