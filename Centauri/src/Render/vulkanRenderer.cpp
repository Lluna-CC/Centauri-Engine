#include "vulkanRenderer.h"
#include "vulkanUtils.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(VulkanPlatform* plat) {
        platform = plat;
		InitializeRenderer();
	}

    void VulkanRenderer::InitializeRenderer() {
	
		graphicsCommandBuffers = platform -> CreateCommandBuffers(MAX_FRAMES_IN_FLIGHT);
    
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
    
    Renderer* Renderer::CreateRenderer(RenderPlatform* plat) {

        return new VulkanRenderer(dynamic_cast<VulkanPlatform*> (plat));
    }
}