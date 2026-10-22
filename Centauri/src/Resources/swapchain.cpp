#include "swapchain.h"

namespace Centauri {
    bool Swapchain::Load() {
        vkPlat -> CreateSwapChain(width, height, swapChain, swapChainExtent, swapChainSurfaceFormat);
        swapChainImages = swapChain.getImages();
        
		swapChainImageViews.reserve(swapChainImages.size());
		for (auto &image : swapChainImages) {
			swapChainImageViews.emplace_back(vkPlat -> CreateImageView(image, swapChainSurfaceFormat.format, vk::ImageAspectFlagBits::eColor, 1));
		}

        return true;
    }

    void Swapchain::Unload() {
        //Unload swapchain ??
    }

    vk::ResultValue<unsigned int> Swapchain::AcquireNextImage(vk::raii::Semaphore& sem) {
        return swapChain.acquireNextImage(UINT64_MAX, *sem, nullptr);
    }

    void Swapchain::CleanSwapchain() {
        swapChainImageViews.clear();
        swapChain = nullptr;
    }

    bool Swapchain::Reload(int w, int h) {
        width = w;
        height = h;
        return Load();
    }
}