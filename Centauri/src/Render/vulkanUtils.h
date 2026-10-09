#include <vulkan/vulkan_raii.hpp>

namespace Centauri {
    vk::raii::ImageView createImageView(const vk::raii::Device& dev, vk::Image const &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t levels) {
		vk::ImageViewCreateInfo imageViewCreateInfo{
			.image = image,
			.viewType = vk::ImageViewType::e2D,
			.format = format,
			.subresourceRange = {.aspectMask = aspectFlags, .levelCount = levels, .layerCount = 1} 	 
		};
	
		return vk::raii::ImageView(dev, imageViewCreateInfo);
    }
    
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDeviceMemoryProperties memProperties) {
		
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
			if ((typeFilter & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & properties) == properties)) return i;
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

    std::pair<vk::raii::Image, vk::raii::DeviceMemory> createImage(const vk::raii::Device& dev, uint32_t width, uint32_t height, uint32_t levels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, uint32_t qIdx, vk::PhysicalDeviceMemoryProperties memProperties) {
		vk::ImageCreateInfo imageInfo {
			.imageType = vk::ImageType::e2D,
			.format = format,
			.extent = {width, height, 1},
			.mipLevels = levels,
			.arrayLayers = 1,
			.samples = numSamples,
			.tiling = tiling, 
			.usage = usage,
			.sharingMode = vk::SharingMode::eConcurrent,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &qIdx
		};

		vk::raii::Image image = vk::raii::Image(dev, imageInfo);
	
		vk::MemoryRequirements memImageRequirements = image.getMemoryRequirements();
		vk::MemoryAllocateInfo allocInfo {
			.allocationSize = memImageRequirements.size, 
			.memoryTypeIndex = findMemoryType(memImageRequirements.memoryTypeBits, properties, memProperties)
		};

		vk::raii::DeviceMemory imageMemory = vk::raii::DeviceMemory(dev, allocInfo);
		image.bindMemory(imageMemory, 0);

		return {std::move(image), std::move(imageMemory)};
	}

    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, const vk::FormatProperties &props) {
		for (vk::Format format : candidates) {
			
			if (((tiling == vk::ImageTiling::eLinear) && ((props.linearTilingFeatures & features) == features)) ||
				((tiling == vk::ImageTiling::eOptimal) && ((props.optimalTilingFeatures & features) == features))) 
					return format;
			
			
		}
		throw std::runtime_error("failed to find supported format");
	}
    
  

}
