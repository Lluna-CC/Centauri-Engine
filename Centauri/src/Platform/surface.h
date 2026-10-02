#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include "../Events/eventSystem.h"

namespace Centauri {

    struct SurfaceProps {
        std::string title;
        unsigned int width;
        unsigned int height;

        SurfaceProps(const std::string& title = "Centauri Engine",
                     unsigned int width = 1280,
                     unsigned int height = 720) : title(title), width(width), height(height) {}
    };

    class Surface {
      public:
        Surface() = default;
        virtual ~Surface() {}

        virtual void Initialize() = 0;
        virtual void OnUpdate() = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual bool Closed() const = 0;
        
        void SetEventSystem(EventSystem* eventSys) {eventSystem = eventSys;}

        //virtual void SetVSync(bool enabled) = 0;
        //virtual bool IsVSync() const = 0;
    
        static Surface* Create(const SurfaceProps& props = SurfaceProps());
      protected:
        unsigned int width;
        unsigned int height;
        std::string title;
        EventSystem* eventSystem;
    };
}

#endif //SURFACE_H