#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "pipeline.h"
#include <vulkan/vulkan_raii.hpp>
#include "vulkanUtils.h"
#include<string>

namespace Centauri {
    class VulkanPipeline : public Pipeline {
      public:
        
      private:
        void CreateGraphicsPipeline(const std::string& shaderFilename);
        
        
        /// Move to resoruces? ////////////////////////////////////
        vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
        vk::raii::PipelineLayout pipelineLayout = nullptr;
	      vk::raii::Pipeline graphicsPipeline = nullptr;
        ///////////////////////////////////////////////////////////
    };
}

#endif //VULKAN_PIPELINE_H