
#pragma once

// Include Plutonium's main header
#include <pu/Plutonium>
#include "Fs.hpp"

class MainApplication;

class CustomLayout : public pu::ui::Layout {
    private:

        // An easy way to keep objects is to have them as private members
        // Using ::Ref (of a Plutonium built-in object or any class having PU_SMART_CTOR) is an alias to a shared_ptr of the instance.
        pu::ui::elm::TextBlock::Ref greeting;
        pu::ui::elm::Menu::Ref partitionSelector;
        MainApplication* app;

    public:

        CustomLayout(MainApplication* app);

        // Have ::Ref alias and ::New() static constructor
        PU_SMART_CTOR(CustomLayout)
};

// Define your application (can't instantiate base class, so need to make a derived one)
class MainApplication : public pu::ui::Application {
    private:

        // Layout instance
        CustomLayout::Ref layout;
    
    public:
        using Application::Application;
        PU_SMART_CTOR(MainApplication)
        ~MainApplication();

        // We need to define this, and use it to initialize everything
        void OnLoad() override;
};
