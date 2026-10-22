#ifndef VULKAN_SURCACE_H
#define VULKAN_SURFACE_H

#include "surface.h"
#include <vulkan/vulkan_raii.hpp>


namespace Centauri {
    class VulkanSurface : public Surface {
      public:
        virtual ~VulkanSurface() {}

        virtual void Initialize() = 0;
        virtual void OnUpdate() = 0;
        virtual unsigned int GetWidth() = 0;
        virtual unsigned int GetHeight() = 0;
        virtual bool Closed() const = 0;

        virtual VkSurfaceKHR GetVulkanSurface(const VkInstance& instance) const = 0;

    };
}

#endif //VULKAN_SURFACE_H