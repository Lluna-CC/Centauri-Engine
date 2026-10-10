#ifndef VULKAN_PLATFORM_H
#define VULKAN_PLATFORM_H

#include <vulkan/vulkan_raii.hpp>
#include "renderPlatform.h"
#include<vector>

namespace Centauri {
    class VulkanPlatform: public RenderPlatform {
      public:
        virtual ~VulkanPlatform();

        virtual void InitializePlatform(VulkanSurface* surf);
        VulkanPlatform(VulkanSurface* surf);
        
        std::vector<vk::raii::CommandBuffer> CreateCommandBuffers(uint count);
        
        void SetFramesInFlight(uint frames_in_flight);
        
      
      private:

        void CreateInstance();
        void ChoosePhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain(int width, int height);
        void CreateCommandPools();
        void CreateSyncObjects(uint frames_in_flight);
    
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

        //Should this be on renderer?
        std::vector<vk::Image> swapChainImages;
        std::vector<vk::raii::ImageView> swapChainImageViews;

        vk::raii::CommandPool graphicsCommandPool = nullptr;

        std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
        std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
        std::vector<vk::raii::Fence> inFlightFences;

        
    };
}

#endif //VULKAN_PLATFORM_H

