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
        
        void CreateSwapChain(int width, int height, VkSwapchainKHR* swapChain, vk::Extent2D& swapChainExtent, 
                              vk::SurfaceFormatKHR& swapChainSurfaceFormat, std::vector<VkImage>& swapChainImages 
        );

        vk::ImageView CreateImageView(VkImage const &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t levels);

        std::pair<vk::raii::Image, vk::raii::DeviceMemory> CreateImage(
          uint32_t width, uint32_t height, uint32_t levels, vk::SampleCountFlagBits numSamples, vk::Format format, 
          vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties
        );
          
      private:

        void CreateInstance();
        void ChoosePhysicalDevice();
        void CreateLogicalDevice();
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
 
        uint32_t swapchainImgCount = 0;
        //std::vector<vk::Image> swapChainImages;
        //std::vector<vk::raii::ImageView> swapChainImageViews;

        vk::raii::CommandPool graphicsCommandPool = nullptr;

        std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
        std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
        std::vector<vk::raii::Fence> inFlightFences;

        
    };
}

#endif //VULKAN_PLATFORM_H

