#include "swapchain.h"

namespace Centauri {
    bool Swapchain::Load() {
        vkPlat -> CreateSwapChain(width, height, swapChain, swapChainExtent, swapChainSurfaceFormat, swapChainImages);
        
		swapChainImageViews.reserve(swapChainImages.size());
		for (auto &image : swapChainImages) {
			swapChainImageViews.emplace_back(vkPlat -> CreateImageView(image, swapChainSurfaceFormat.format, vk::ImageAspectFlagBits::eColor, 1));
		}

        for (uint i = 0; i < swapChainImages.size(); ++i) {
			renderFinishedSemaphores.push_back(vkPlat -> CreateSemaphore(vk::SemaphoreCreateInfo()));
		}
        
        return true;
    }

    void Swapchain::Unload() {
        //Unload swapchain ??
    }
}