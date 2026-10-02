#ifndef VULKAN_PLATFORM_H
#define VULKAN_PLATFORM_H

#include <vulkan/vulkan_raii.hpp>
#include "renderPlatform.h"

namespace Centauri {
    class VulkanPlatform: public RenderPlatform {
      public:
        virtual ~VulkanPlatform();

        virtual void InitializePlatform();

      private:

        void CreateInstance();
        void CreateSurface();
        void ChoosePhysicalDevice();
        void CreateLogicalDevice();
        
        vk::raii::Context context;
        vk::raii::Instance inst = nullptr;
        
        vk::raii::PhysicalDevice physicalDev = nullptr;
	      vk::raii::Device dev = nullptr;
        uint32_t graphicsQueueIndex = ~0;
	      vk::raii::Queue graphicsQueue = nullptr;
        
        vk::raii::SurfaceKHR surface = nullptr;
        
    };
}

#endif //VULKAN_PLATFORM_H

