#include "vulkanRenderer.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(VulkanPlatform* plat, ResourceManager* resMngr) {
        platform = plat;
		resourceManager = resMngr;

		InitializeRenderer();
	}

    void VulkanRenderer::InitializeRenderer() {
	
		graphicsCommandBuffers = platform -> CreateCommandBuffers(MAX_FRAMES_IN_FLIGHT);
    
		swapchain = resourceManager -> Load<Swapchain>("main_swapchain", platform, 0, 0);
		//colorAttachment = resourceManager -> Load<RenderAttachment>();
		/*
	
		dev = plat.GetDevice();
		swapChain = plat.GetSwapChain();
		renderQueue = plat.GetGraphicsQueue();
		swapChainExtent = plat.GetSwapChainExtent();
		swapChainSurfaceFormat = plat.GetSwapChainSurfaceFormat();
		swapChainImages = swapChain.getImages();
		*/

		//createImageViews();
		//createCommandPools(plat.GetGraphicsQueueIdx());
		//createColorResources(plat.GetGraphicsQueueIdx(), plat.GetMemoryProperties());
	}
    
    Renderer* Renderer::CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr) {

        return new VulkanRenderer(dynamic_cast<VulkanPlatform*> (plat), resMngr);
    }
}