#include "vulkanRenderer.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(VulkanPlatform* plat, ResourceManager* resMngr) {
        platform = plat;
		resourceManager = resMngr;

		InitializeRenderer();
	}

    void VulkanRenderer::InitializeRenderer() {
	
		graphicsCommandBuffers = platform -> CreateCommandBuffers(MAX_FRAMES_IN_FLIGHT);
    
		
		/*
		renderQueue = plat.GetGraphicsQueue();
		*/
	}

	void VulkanRenderer::SetWindowSize(int width, int height) {
		
		swapchain = resourceManager -> Load<Swapchain>("main_swapchain", platform, width, height);
		
		colorAttachment = resourceManager -> Load<RenderAttachment>(
			"Color_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
			swapchain -> GetFormat().format
		);

		vk::Format depthFormat = platform -> findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
		depthAttachment = resourceManager -> Load<RenderAttachment>(
			"Depth_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eDepthStencilAttachment,
			depthFormat
		);
	}

    
    Renderer* Renderer::CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr) {

        return new VulkanRenderer(dynamic_cast<VulkanPlatform*> (plat), resMngr);
    }
}