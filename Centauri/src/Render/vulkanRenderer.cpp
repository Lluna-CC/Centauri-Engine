#include "vulkanRenderer.h"
#include "vulkanUtils.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(VulkanPlatform& plat) {
        InitializeRenderer(plat);
    }

    void VulkanRenderer::InitializeRenderer(VulkanPlatform& plat) {
        
		/*
	
		dev = plat.GetDevice();
		swapChain = plat.GetSwapChain();
		renderQueue = plat.GetGraphicsQueue();
		swapChainExtent = plat.GetSwapChainExtent();
		swapChainSurfaceFormat = plat.GetSwapChainSurfaceFormat();
		swapChainImages = swapChain.getImages();
		*/

		createImageViews();
		//createCommandPools(plat.GetGraphicsQueueIdx());
		//createColorResources(plat.GetGraphicsQueueIdx(), plat.GetMemoryProperties());
	}

    
    
    void VulkanRenderer::createImageViews() {
        assert(swapChainImageViews.empty());

		swapChainImageViews.reserve(swapChainImages.size());
		for (auto &image : swapChainImages) {
			swapChainImageViews.emplace_back(createImageView(dev, image, swapChainSurfaceFormat.format, vk::ImageAspectFlagBits::eColor, 1));
		}
    }
    void VulkanRenderer::createCommandPools(uint32_t graphicsQueueIndex) {
        vk::CommandPoolCreateInfo gPoolInfo {
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = graphicsQueueIndex
		};
		graphicsCommandPool = vk::raii::CommandPool(dev, gPoolInfo);
    }

    void VulkanRenderer::createColorResources(uint32_t graphicsQueueIndex, vk::PhysicalDeviceMemoryProperties& memProperties) {
        vk::Format colorFormat = swapChainSurfaceFormat.format;

		std::tie(colorImage, colorImageMemory) = createImage(dev, swapChainExtent.width, swapChainExtent.height, 1, vk::SampleCountFlagBits::e1, colorFormat, 
															vk::ImageTiling::eOptimal,
															vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, 
															vk::MemoryPropertyFlagBits::eDeviceLocal, graphicsQueueIndex, memProperties);
		colorImageView = createImageView(dev, colorImage, colorFormat, vk::ImageAspectFlagBits::eColor, 1);
    }

    void VulkanRenderer::createDepthResources(const vk::FormatProperties &props, uint32_t graphicsQueueIndex, vk::PhysicalDeviceMemoryProperties& memProperties) {
        
        vk::Format depthFormat = findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment, props);
		std::tie(depthImage, depthImageMemory) = createImage(dev, swapChainExtent.width, swapChainExtent.height, 1, vk::SampleCountFlagBits::e1,depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, graphicsQueueIndex, memProperties);
		depthImageView = createImageView(dev, depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);
        
    }

    void VulkanRenderer::createCommandBuffers() {
        vk::CommandBufferAllocateInfo gAllocInfo{
			.commandPool = graphicsCommandPool,
			.level = vk::CommandBufferLevel::ePrimary, 
			.commandBufferCount = VulkanRenderer::MAX_FRAMES_IN_FLIGHT
		};

		graphicsCommandBuffers = vk::raii::CommandBuffers(dev,gAllocInfo);
    }

    void VulkanRenderer::createSyncObjects() {
        assert(presentCompleteSemaphores.empty() && renderFinishedSemaphores.empty() && inFlightFences.empty());
		for (uint i = 0; i < VulkanRenderer::MAX_FRAMES_IN_FLIGHT; ++i) {
			presentCompleteSemaphores.emplace_back(dev, vk::SemaphoreCreateInfo());
			inFlightFences.emplace_back(dev, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled});
		}
		for (uint i = 0; i < swapChainImages.size(); ++i) {
			renderFinishedSemaphores.emplace_back(dev, vk::SemaphoreCreateInfo());
		}
    }
    
    Renderer* Renderer::CreateRenderer(RenderPlatform& plat) {

        return new VulkanRenderer(*dynamic_cast<VulkanPlatform*> (&plat));
    }
}