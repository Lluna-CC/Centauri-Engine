#ifndef RENDERER_H
#define RENDERER_H

#include "../Platform/renderPlatform.h"
#include "../Resources/resource.h"

namespace Centauri {
    class Renderer {
      public:
        virtual ~Renderer() = default;

        static Renderer* CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr);
      protected:
        
        
    };
}

#endif //RENDERER_H