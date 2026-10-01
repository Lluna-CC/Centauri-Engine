#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan_raii.hpp>

namespace Centauri {
    class VulkanRenderer: public Renderer {
      public:


      private:
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
        std::vector<vk::Image> swapChainImages;
        std::vector<vk::raii::ImageView> swapChainImageViews;
        
        vk::raii::PipelineLayout pipelineLayout = nullptr;
	    vk::raii::Pipeline graphicsPipeline = nullptr;

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

