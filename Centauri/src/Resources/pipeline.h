#ifndef PIPELINE_H
#define PIPELINE_H

#include "resource.h"
#include "../Platform/vulkanPlatform.h"
#include "../Render/vulkanUtils.h"

namespace Centauri {
    
    class Pipeline : public Resource {
      public:
        
        explicit Pipeline(
            const std::string& id, VulkanPlatform* plat, const std::string& shader, vk::VertexInputBindingDescription bindingDesc,
            uint32_t attribCount, vk::VertexInputAttributeDescription* attributeDescs, vk::SampleCountFlagBits msaa = vk::SampleCountFlagBits::e1 
        ) : Resource(id), vkPlat(plat), shaderFile(shaderFile), bindingDescription(bindingDesc), 
            attribCount(attribCount), attributeDescriptions(attributeDescs), msaaSamples(msaa) {}

        ~Pipeline() {
            Unload();
        }

        bool Load() override;
        void Unload() override;
      
      private:
        vk::raii::Pipeline vkPipeline = nullptr; 
        vk::raii::PipelineLayout pipelineLayout = nullptr;
        vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;

        vk::SampleCountFlagBits msaaSamples;
        std::string shaderFile;
        VulkanPlatform* vkPlat = nullptr;
        
        vk::VertexInputBindingDescription bindingDescription;
        uint32_t attribCount;
        vk::VertexInputAttributeDescription* attributeDescriptions;
    };
}

#endif //PIPELINE_H