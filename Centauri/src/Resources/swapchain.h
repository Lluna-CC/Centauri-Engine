#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "resource.h"
#include "../Platform/vulkanPlatform.h"

namespace Centauri {
    class Swapchain : public Resource {
        public:
            explicit Swapchain(const std::string& id, VulkanPlatform* plat, int w = 0, int h = 0) : Resource(id), vkPlat(plat), width(w), height(h) {}
            
            ~Swapchain() override {
                Unload();
            }

            bool Load() override;
            void Unload() override;

            vk::Extent2D GetExtent() {return swapChainExtent;}
            vk::SurfaceFormatKHR GetFormat() {return swapChainSurfaceFormat;}

            vk::ResultValue<unsigned int> AcquireNextImage(vk::raii::Semaphore& sem);

            vk::Image& GetImage(size_t imageIdx) {return swapChainImages[imageIdx];}
            vk::raii::ImageView& GetImageView(size_t imageIdx) {return swapChainImageViews[imageIdx];}
            
            size_t GetImageCount() {return swapChainImages.size();}
            vk::raii::SwapchainKHR& GetSwapchain() {return swapChain;}

            void CleanSwapchain();
            bool Reload(int w, int h);

        private:
            vk::raii::SwapchainKHR swapChain = nullptr;
            vk::Extent2D swapChainExtent;
            vk::SurfaceFormatKHR swapChainSurfaceFormat;

            std::vector<vk::Image> swapChainImages;
            std::vector<vk::raii::ImageView> swapChainImageViews;
                
            VulkanPlatform* vkPlat;

            int width;
            int height;
    };
}

#endif //SWAPCHAIN_H