#include "application.h"
#include "../Events/windowEvent.h"
#include <iostream>

constexpr uint32_t WIDTH  = 800;
constexpr uint32_t HEIGHT = 600;

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
        if (e.IsInCategory(Centauri::EventCategory::Window)) {
            std::cout << "Window event !" << std::endl;
            EventDispatcher dispatcher(e);

            dispatcher.Dispatch<WindowResizedEvent> ([this](const WindowResizedEvent& ev){
                int w = ev.GetWidth();
                int h = ev.GetHeight();

                platform -> WaitDevice();
                renderer -> WindowResize(w, h);
            });
            
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

        ResourceManager* resourceManager = new ResourceManager;
        renderer = Renderer::CreateRenderer(platform, resourceManager);

        renderer -> InitializeWindow(surface -> GetWidth(), surface -> GetHeight());

        while (!(surface -> Closed())) {
            surface -> OnUpdate();
            renderer -> StartRendering();
            renderer -> FinishRendering();
        }
    }

    Application::~Application() {
        delete surface;
        delete platform;
    }

    
}