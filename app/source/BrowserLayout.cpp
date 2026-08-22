#include "BrowserLayout.hpp"
#include "MainApplication.hpp"

namespace tmc {
namespace ExplorerNX {

BrowserLayout::BrowserLayout(MainApplication* app, const std::filesystem::path& currentDirectory) : pu::ui::Layout::Layout()
{
    this->app = app;
    this->currentDirectory = currentDirectory;
    this->directoryView = pu::ui::elm::Menu::New(0, 50, 1920, pu::ui::Color(0, 0, 127, 255), pu::ui::Color(127, 127, 255, 255), 128, 8);
    this->controlsRibbon = pu::ui::elm::TextBlock::New(10, 10, "A: Enter focused directory | B: Up one level | Y: Multiselect | X: Context menu");
    this->directoryIcon = pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/directory.png"));
    this->directorySelIcon = pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/directory_sel.png"));
    this->fileIcon = pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/file.png"));
    this->fileSelIcon = pu::sdl2::TextureHandle::New(pu::ui::render::LoadImageFromFile("romfs:/file_sel.png"));

    for (auto& p: std::filesystem::directory_iterator(this->currentDirectory))
    {
        pu::ui::elm::MenuItem::Ref entry = pu::ui::elm::MenuItem::New(p.path().filename().string());
        if (std::filesystem::is_directory(p.path()))
        {
            entry->SetIcon(directoryIcon);
            entry->AddOnKey([this, p]{
                BrowserLayout::Ref chosenDirectoryView = BrowserLayout::New(this->app, p.path());
                this->app->LoadLayout(chosenDirectoryView);
            });
        }
        else
        {
            entry->SetIcon(fileIcon);
        }
        entry->AddOnKey([this]{
            if (this->currentDirectory.parent_path().string().back() == ':' && this->currentDirectory.string().back() == '/')
            {
                this->app->resetLayout();
            }
            else if (this->currentDirectory.string().back() != '/' && this->currentDirectory.parent_path().string().back() != ':')
            {
                BrowserLayout::Ref chosenDirectoryView = BrowserLayout::New(this->app, this->currentDirectory.parent_path());
                this->app->LoadLayout(chosenDirectoryView);
            }
            else
            {
                BrowserLayout::Ref chosenDirectoryView = BrowserLayout::New(this->app, std::filesystem::path(this->currentDirectory.parent_path().string() + "/"));
                this->app->LoadLayout(chosenDirectoryView);
            }
        }, HidNpadButton_B);
        entry->AddOnKey([this]{
            std::stringstream debugText;

            debugText << "Current Directory: " << this->currentDirectory.string() << std::endl << std::endl;
            debugText << "Has Parent Path: " << std::boolalpha << this->currentDirectory.has_parent_path() << std::endl << std::endl;
            if (this->currentDirectory.has_parent_path())
                debugText << "Parent Path: " << this->currentDirectory.parent_path() << std::endl << std::endl;
            debugText << "Root Name: " << this->currentDirectory.root_name() << std::endl << std::endl;
            debugText << "Filename: " << this->currentDirectory.filename();

            this->app->CreateShowDialog("Debug dialog", debugText.str(), {"Cancel"}, true);
        }, HidNpadButton_Minus);

        this->directoryView->AddItem(entry);
    }
    if (this->directoryView->GetItems().empty())
    {
        pu::ui::elm::MenuItem::Ref dummy = pu::ui::elm::MenuItem::New("(Empty Directory)");

        dummy->AddOnKey([this]{
            if (this->currentDirectory.parent_path().string().back() == ':' && this->currentDirectory.string().back() == '/')
            {
                this->app->resetLayout();
            }
            else if (this->currentDirectory.string().back() != '/' && this->currentDirectory.parent_path().string().back() != ':')
            {
                BrowserLayout::Ref chosenDirectoryView = BrowserLayout::New(this->app, this->currentDirectory.parent_path());
                this->app->LoadLayout(chosenDirectoryView);
            }
            else
            {
                BrowserLayout::Ref chosenDirectoryView = BrowserLayout::New(this->app, std::filesystem::path(this->currentDirectory.parent_path().string() + "/"));
                this->app->LoadLayout(chosenDirectoryView);
            }
        }, HidNpadButton_B);

        this->directoryView->AddItem(dummy);
    }
    this->Add(this->controlsRibbon);
    this->Add(this->directoryView);
}

}
}
