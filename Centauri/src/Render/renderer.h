#ifndef RENDERER_H
#define RENDERER_H

namespace Centauri {
    class Renderer {
      public:
        virtual ~Renderer() = default;

        static Renderer* CreateRenderer();
    };
}

#endif //RENDERER_H