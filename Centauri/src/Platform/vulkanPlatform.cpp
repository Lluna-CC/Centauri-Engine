#include "vulkanPlatform.h"

namespace Centauri {

    VulkanPlatform::~VulkanPlatform() {

    }

    void VulkanPlatform::InitializePlatform() {
        CreateInstance();
        CreateSurface();
        ChoosePhysicalDevice();
        CreateLogicalDevice();
    }

    void VulkanPlatform::CreateInstance() {

    }

    void VulkanPlatform::CreateSurface() {

    }

    void VulkanPlatform::ChoosePhysicalDevice() {

    }

    void VulkanPlatform::CreateLogicalDevice() {

    }

    RenderPlatform* RenderPlatform::CreateRenderPlatform(const VulkanSurface& surf) {
        
    }

        
}