#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include "resource.h"

namespace Centauri {
    class VertexBuffer : public Resource {
      public:
        explicit VertexBuffer(const std::string& id) : Resource(id) {}

        ~VertexBuffer() override {
            Unload();
        }

        bool Load() override;
        void Unload() override;
    
      private:
        vk::raii::Buffer vertexBuffer = nullptr;
	    vk::raii::DeviceMemory vertexBufferMemory = nullptr;
        
        //Should we move this?
        vk::raii::Buffer indexBuffer = nullptr;
	    vk::raii::DeviceMemory indexBufferMemory = nullptr;
	
    };

}

#endif //VERTEXBUFFER_H