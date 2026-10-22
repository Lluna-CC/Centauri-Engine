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
        
        void CreateSwapChain(int width, int height, vk::raii::SwapchainKHR& swapChain, vk::Extent2D& swapChainExtent, vk::SurfaceFormatKHR& swapChainSurfaceFormat);

        vk::raii::ImageView CreateImageView(vk::Image const &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t levels);

        std::pair<vk::raii::Image, vk::raii::DeviceMemory> CreateImage(
          uint32_t width, uint32_t height, uint32_t levels, vk::SampleCountFlagBits numSamples, vk::Format format, 
          vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties
        );

        vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;
        [[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;
        vk::raii::PipelineLayout CreatePipelineLayout(vk::PipelineLayoutCreateInfo pipeLayoutInfo) {return vk::raii::PipelineLayout(dev, pipeLayoutInfo);}
        vk::raii::Pipeline CreatePipeline(vk::GraphicsPipelineCreateInfo graphicsInfo) {return vk::raii::Pipeline(dev, nullptr, graphicsInfo);}
        vk::raii::Semaphore CreateSemaphore(vk::SemaphoreCreateInfo info) {return vk::raii::Semaphore(dev, info);}

        void SubmitQueue(const vk::SubmitInfo& submitInfo, size_t frameIdx);
        vk::Result Present(const vk::PresentInfoKHR& presentInfo);

        void WaitForFence(uint fenceIdx);
        void ResetFence(uint fenceIdx);

      private:

        void CreateInstance();
        void SetupDebugMessenger();
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

        vk::raii::CommandPool graphicsCommandPool = nullptr;

        std::vector<vk::raii::Fence> inFlightFences;

        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

        
    };
}

#endif //VULKAN_PLATFORM_H

