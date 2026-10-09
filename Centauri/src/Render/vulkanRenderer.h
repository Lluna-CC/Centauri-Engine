#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan_raii.hpp>
#include "renderer.h"
#include <vector>
#include "../Platform/vulkanPlatform.h"

namespace Centauri {
    class VulkanRenderer : public Renderer {
      public:
        VulkanRenderer(VulkanPlatform& plat);

      private:
        void InitializeRenderer(VulkanPlatform& plat);
      
        void createImageViews();
        void createCommandPools(uint32_t graphicsQueueIndex);
		void createColorResources(uint32_t graphicsQueueIndex, vk::PhysicalDeviceMemoryProperties& memProperties);
		void createDepthResources(const vk::FormatProperties &props, uint32_t graphicsQueueIndex, vk::PhysicalDeviceMemoryProperties& memProperties);
        void createCommandBuffers();
		void createSyncObjects();

        static const int MAX_FRAMES_IN_FLIGHT = 2;

        vk::raii::Device dev = nullptr;
        
        vk::raii::Queue renderQueue = nullptr;
        vk::raii::SwapchainKHR swapChain = nullptr;
        vk::Extent2D swapChainExtent;
        vk::SurfaceFormatKHR swapChainSurfaceFormat;
        std::vector<vk::Image> swapChainImages;
        std::vector<vk::raii::ImageView> swapChainImageViews;

	    vk::raii::CommandPool graphicsCommandPool = nullptr;
        std::vector<vk::raii::CommandBuffer> graphicsCommandBuffers;
        
        std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
        std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
        std::vector<vk::raii::Fence> inFlightFences;
                
        uint32_t frameIndex = 0;
        bool framebufferResized = false;

        vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;
        vk::raii::Image colorImage = nullptr;
        vk::raii::DeviceMemory colorImageMemory = nullptr;
        vk::raii::ImageView colorImageView = nullptr;
        
        vk::raii::Image depthImage = nullptr;
        vk::raii::DeviceMemory depthImageMemory = nullptr;
        vk::raii::ImageView depthImageView = nullptr;
    
    };
}

#endif //VULKAN_RENDERER_H