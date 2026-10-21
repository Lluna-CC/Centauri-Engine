
#include "vulkanUtils.h"

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

	void transition_image_layout(
		const vk::Image				&image,
		vk::ImageLayout			old_layout,
		vk::ImageLayout			new_layout,
		vk::AccessFlags2		src_access_mask,
		vk::AccessFlags2		dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask,
		vk::ImageAspectFlags	image_aspect_flags,
		vk::raii::CommandBuffer& commandBuffer
	) 
	{
		vk::ImageMemoryBarrier2 barrier = {
			.srcStageMask		= src_stage_mask,
			.srcAccessMask		= src_access_mask,
			.dstStageMask		= dst_stage_mask,
			.dstAccessMask 		= dst_access_mask,
			.oldLayout			= old_layout,
			.newLayout 			= new_layout,
			.srcQueueFamilyIndex= VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex= VK_QUEUE_FAMILY_IGNORED,
			.image				= image,
			.subresourceRange 	=	{
					.aspectMask		= image_aspect_flags,
					.baseMipLevel	= 0,
					.levelCount		= 1,
					.baseArrayLayer = 0,
					.layerCount 	= 1
					}
		};

		vk::DependencyInfo dependency_info = {
			.dependencyFlags			= {},
			.imageMemoryBarrierCount 	= 1,
			.pImageMemoryBarriers		= &barrier
		};

		commandBuffer.pipelineBarrier2(dependency_info);
		
	}

}

