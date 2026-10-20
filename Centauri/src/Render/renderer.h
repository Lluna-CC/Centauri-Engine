#ifndef RENDERER_H
#define RENDERER_H

#include "../Platform/renderPlatform.h"
#include "../Resources/resource.h"

namespace Centauri {
    class Renderer {
      public:
        virtual ~Renderer() = default;

        static Renderer* CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr);

        virtual void SetWindowSize(int width, int height) = 0;
      
      protected:
        
        
    };
}

#endif //RENDERER_H