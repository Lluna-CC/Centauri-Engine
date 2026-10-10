#ifndef RENDER_PLATFORM_H
#define RENDER_PLATFORM_H

#include "../Platform/vulkanGLFWWindow.h"

namespace Centauri {
    class RenderPlatform {
      public:
        virtual ~RenderPlatform() = default;

        static RenderPlatform* CreateRenderPlatform(Surface* surf);
    };
}

#endif //RENDERER_PLATFORM_H