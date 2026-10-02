#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

namespace Centauri {
    class VulkanPipeline : public Pipeline {
      public:
        
      private:
        vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
        vk::raii::PipelineLayout pipelineLayout = nullptr;
	    vk::raii::Pipeline graphicsPipeline = nullptr;
    };
}

#endif //VULKAN_PIPELINE_H