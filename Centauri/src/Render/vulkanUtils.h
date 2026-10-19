#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#include <vulkan/vulkan_raii.hpp>
#include <fstream>

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
    
    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, const vk::FormatProperties &props) {
		for (vk::Format format : candidates) {
			
			if (((tiling == vk::ImageTiling::eLinear) && ((props.linearTilingFeatures & features) == features)) ||
				((tiling == vk::ImageTiling::eOptimal) && ((props.optimalTilingFeatures & features) == features))) 
					return format;
			
			
		}
		throw std::runtime_error("failed to find supported format");
	}
    
	void readFile(const std::string& filename, std::vector<char>& buffer) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		
		buffer.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

		file.close();
	}

}

#endif //VULKAN_UTILS_H