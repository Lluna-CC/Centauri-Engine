#include "vulkanRenderer.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(const VulkanPlatform& plat) {
        InitializeRenderer(plat);
    }

    void VulkanRenderer::InitializeRenderer(const VulkanPlatform& plat) {
        
    }

    void VulkanRenderer::createSwapChain() {
        /*
        vk::SurfaceCapabilitiesKHR surfaceCaps = physicalDev.getSurfaceCapabilitiesKHR(*surface);
		swapChainExtent = chooseSwapExtent(surfaceCaps);

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			swapChainExtent = capabilities.currentExtent;
		}	
        
        int w = 100;
        int h = 200;

		swapChainExtent = {
			std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};

		
        uint32_t minImageCount = chooseSwapMinImageCount(surfaceCaps);

		std::vector<vk::SurfaceFormatKHR> availableFormats = physicalDev.getSurfaceFormatsKHR(*surface);
		swapChainSurfaceFormat = chooseSwapSurfaceFormat(availableFormats);

		std::vector<vk::PresentModeKHR> availablePresents = physicalDev.getSurfacePresentModesKHR(*surface);

		vk::SwapchainCreateInfoKHR swapChainCreateInfo{
			.surface = *surface,
			.minImageCount = minImageCount,
			.imageFormat = swapChainSurfaceFormat.format,
			.imageColorSpace = swapChainSurfaceFormat.colorSpace,
			.imageExtent = swapChainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = surfaceCaps.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = chooseSwapPresentMode(availablePresents),
			.clipped = true,
			.oldSwapchain = nullptr

		};

		swapChain = vk::raii::SwapchainKHR(dev, swapChainCreateInfo);
		swapChainImages = swapChain.getImages();
        */
    }
    
    void VulkanRenderer::createImageViews() {

    }
    void VulkanRenderer::createCommandPools() {

    }
    void VulkanRenderer::createColorResources() {

    }
    void VulkanRenderer::createDepthResources() {

    }

    void VulkanRenderer::createCommandBuffers() {

    }

    void VulkanRenderer::createSyncObjects() {

    }
    
    Renderer* Renderer::CreateRenderer(RenderPlatform& plat) {
        //VulkanRenderer vkRender;
        //vkRender.Initialize(); 
        return nullptr;
    }
}