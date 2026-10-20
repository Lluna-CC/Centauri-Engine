#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan_raii.hpp>
#include "renderer.h"
#include <vector>
#include "../Platform/vulkanPlatform.h"
#include "pipeline.h"
#include "../Resources/swapchain.h"
#include "../Resources/renderAttachment.h"

namespace Centauri {
    class VulkanRenderer : public Renderer {
      public:
        VulkanRenderer(VulkanPlatform* plat, ResourceManager* resMngr);

        virtual void SetWindowSize(int width, int height) override;
        //void SetPipeline(Pipeline* pipe) {pipeline = pipe}

      private:
        void InitializeRenderer();
		  
        //Should we use shared pointers?
        VulkanPlatform* platform;
        ResourceManager* resourceManager;
        
        static const int MAX_FRAMES_IN_FLIGHT = 2;

        //Move these to resorces 
        ResourceHandle<Swapchain> swapchain;
        ResourceHandle<RenderAttachment> colorAttachment;
        ResourceHandle<RenderAttachment> depthAttachment;

        std::vector<vk::raii::CommandBuffer> graphicsCommandBuffers;
                
        uint32_t frameIndex = 0;
        bool framebufferResized = false;

        vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

        //Pipeline* pipeline;

        
        
    };
}

#endif //VULKAN_RENDERER_H