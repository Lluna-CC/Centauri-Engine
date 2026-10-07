#include "vulkanRenderer.h"
#include "vulkanUtils.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(const VulkanPlatform& plat) {
        InitializeRenderer(plat);
    }

    void VulkanRenderer::InitializeRenderer(const VulkanPlatform& plat) {
        
    }

    void VulkanRenderer::createSwapChain(vk::raii::SurfaceKHR& surface, vk::SurfaceCapabilitiesKHR const &capabilities, std::vector<vk::SurfaceFormatKHR> const &availableFormats, std::vector<vk::PresentModeKHR> const &availablePresents, int width, int height) {
        
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			swapChainExtent = capabilities.currentExtent;
		}
        
		swapChainExtent = vk::Extent2D {
			std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};

        auto minImageCount = std::max(3u, capabilities.minImageCount);
		if ((0 < capabilities.maxImageCount) && (capabilities.maxImageCount < minImageCount)) {
			minImageCount = capabilities.maxImageCount;
		}

        const auto formatIt = std::ranges::find_if (availableFormats, [](const auto &format) {return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;}); 
		if (formatIt != availableFormats.end()) swapChainSurfaceFormat = *formatIt;
		else swapChainSurfaceFormat = availableFormats[0];

        vk::PresentModeKHR present;
		vk::SwapchainCreateInfoKHR swapChainCreateInfo{
			.surface = *surface,
			.minImageCount = minImageCount,
			.imageFormat = swapChainSurfaceFormat.format,
			.imageColorSpace = swapChainSurfaceFormat.colorSpace,
			.imageExtent = swapChainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = capabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = present,
			.clipped = true,
			.oldSwapchain = nullptr

		};

		swapChain = vk::raii::SwapchainKHR(dev, swapChainCreateInfo);
		swapChainImages = swapChain.getImages();
    
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

    void VulkanRenderer::createColorResources(uint32_t graphicsQueueIndex) {
        vk::Format colorFormat = swapChainSurfaceFormat.format;

		std::tie(colorImage, colorImageMemory) = createImage(dev, swapChainExtent.width, swapChainExtent.height, 1, vk::SampleCountFlagBits::e1, colorFormat, 
															vk::ImageTiling::eOptimal,
															vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, 
															vk::MemoryPropertyFlagBits::eDeviceLocal, graphicsQueueIndex);
		colorImageView = createImageView(dev, colorImage, colorFormat, vk::ImageAspectFlagBits::eColor, 1);
    }

    void VulkanRenderer::createDepthResources(const vk::FormatProperties &props, uint32_t graphicsQueueIndex) {
        
        vk::Format depthFormat = findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment, props);
		std::tie(depthImage, depthImageMemory) = createImage(dev, swapChainExtent.width, swapChainExtent.height, 1, vk::SampleCountFlagBits::e1,depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, graphicsQueueIndex);
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
        //VulkanRenderer vkRender;
        //vkRender.Initialize(); 
        return nullptr;
    }
}