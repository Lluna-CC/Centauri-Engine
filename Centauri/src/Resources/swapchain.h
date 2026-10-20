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

        private:
            VkSwapchainKHR* swapChain;
            vk::Extent2D swapChainExtent;
            vk::SurfaceFormatKHR swapChainSurfaceFormat;

            std::vector<VkImage> swapChainImages;
            std::vector<vk::ImageView> swapChainImageViews;
            
            std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
                
            VulkanPlatform* vkPlat;

            int width;
            int height;
    };
}

#endif //SWAPCHAIN_H