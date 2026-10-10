#include "application.h"


constexpr uint32_t WIDTH  = 800;
constexpr uint32_t HEIGHT = 600;

#include <iostream>


namespace Centauri {
   

    void Application::Init() {
    }

    void Application::OnEvent(const Event& e) {
        
        if (e.IsInCategory(Centauri::EventCategory::Input)) {
            std::cout << "IS INPUT !" << std::endl;
        }
        if (e.IsInCategory(Centauri::EventCategory::Keyboard)) {
            std::cout << "IS KEYBOARD !" << std::endl;
        }
        if (e.IsInCategory(Centauri::EventCategory::Mouse)) {
            std::cout << "IS MOUSE !" << std::endl;
        }
    }
    
    void Application::Run() {
        std::cout << "Centauri Engine !!!" << std::endl;

        surface = Surface::CreateSurface();
        std::cout << surface -> GetHeight() << std::endl;
        EventSystem* eSys = new EventSystem();
        eSys -> AddListener(this);
        surface -> SetEventSystem(eSys);

        //ADD ERROR CORRECTION
        platform = RenderPlatform::CreateRenderPlatform(surface);
        
        renderer = Renderer::CreateRenderer(platform);

        while (!(surface -> Closed())) {

        }
    }

    Application::~Application() {
        delete surface;
        delete platform;
    }

    
}