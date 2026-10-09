#ifndef VULKAN_PLATFORM_H
#define VULKAN_PLATFORM_H

#include <vulkan/vulkan_raii.hpp>
#include "renderPlatform.h"
#include<vector>

namespace Centauri {
    class VulkanPlatform: public RenderPlatform {
      public:
        virtual ~VulkanPlatform();

        virtual void InitializePlatform(const VulkanSurface& surf);
        VulkanPlatform(const VulkanSurface& surf);
        
        /*
        vk::raii::Device& GetDevice() {return dev;}
        vk::raii::Queue& GetGraphicsQueue() {return graphicsQueue;}
        vk::raii::SwapchainKHR& GetSwapChain() {return swapChain;}
        vk::Extent2D& GetSwapChainExtent() {return swapChainExtent;}
        vk::SurfaceFormatKHR& GetSwapChainSurfaceFormat() {return swapChainSurfaceFormat;}
      */
      
      private:

        void CreateInstance();
        void ChoosePhysicalDevice();
        void CreateLogicalDevice();
        void createSwapChain(int width, int height);

        virtual std::vector<const char*> RequiredInstanceExtensions();
        
        vk::raii::Context context;
        vk::raii::Instance inst = nullptr;
        
        vk::raii::PhysicalDevice physicalDev = nullptr;
	      vk::raii::Device dev = nullptr;
        uint32_t graphicsQueueIndex = ~0;
	      vk::raii::Queue graphicsQueue = nullptr;
        
        vk::raii::SurfaceKHR surface = nullptr;
        vk::raii::SwapchainKHR swapChain = nullptr;
        vk::Extent2D swapChainExtent;
        vk::SurfaceFormatKHR swapChainSurfaceFormat;
        
    };
}

#endif //VULKAN_PLATFORM_H

