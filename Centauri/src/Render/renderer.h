#ifndef RENDERER_H
#define RENDERER_H

#include "../Platform/renderPlatform.h"

namespace Centauri {
    class Renderer {
      public:
        virtual ~Renderer() = default;

        static Renderer* CreateRenderer(RenderPlatform* plat);
      private:
        
    };
}

#endif //RENDERER_H