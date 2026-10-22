#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <fstream>

namespace Centauri {
    vk::raii::ImageView createImageView(const vk::raii::Device& dev, vk::Image const &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t levels);
    
    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, const vk::FormatProperties &props);
    
	void readFile(const std::string& filename, std::vector<char>& buffer) ;

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
	);

	VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
														  vk::DebugUtilsMessageTypeFlagsEXT type,
														  const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

}

