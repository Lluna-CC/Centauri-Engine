#ifndef RENDERER_H
#define RENDERER_H

#include "../Platform/renderPlatform.h"
#include "../Resources/resource.h"
#include "../Resources/pipeline.h"
#include "../Resources/vertexBuffer.h"


namespace Centauri {
    class Renderer {
      public:
        virtual ~Renderer() = default;

        static Renderer* CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr);

        virtual void SetWindowSize(int width, int height) = 0;

        virtual void StartRendering() = 0;
        //virtual void RenderObject() = 0;
        virtual void FinishRendering() = 0;
        virtual void BindPipeline(ResourceHandle<Pipeline> pipeline) = 0;
        virtual void BindVertexBuffer(ResourceHandle<VertexBuffer>  vbo) = 0;
      
      protected:
        
        
    };
}

#endif //RENDERER_H