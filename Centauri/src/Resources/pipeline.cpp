#include "pipeline.h"

namespace Centauri {
    bool Pipeline::Load() {
        std::vector<char> shaderCode;
        readFile(shaderFile, shaderCode);

		vk::raii::ShaderModule shaderModule = vkPlat -> createShaderModule(shaderCode);
		
        vk::PipelineShaderStageCreateInfo vertShaderStageInfo {.stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule, .pName = "vertMain"};
		vk::PipelineShaderStageCreateInfo fragShaderStageInfo {.stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = "fragMain"};
		
		vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		/*vk::PipelineVertexInputStateCreateInfo vertexInputInfo {
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &bindingDescription,
			.vertexAttributeDescriptionCount = attribCount,
			.pVertexAttributeDescriptions = attributeDescriptions
		};*/

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly {.topology = vk::PrimitiveTopology::eTriangleList};
		
		//vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(swapChainExtent.width), static_cast<float>(swapChainExtent.height), 0.0f, 1.0f};
		//vk::Rect2D	scissor(vk::Offset2D{0,0}, swapChainExtent);
		
		std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
		vk::PipelineDynamicStateCreateInfo dynamicState{.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data()};
		vk::PipelineViewportStateCreateInfo viewportState{.viewportCount = 1, .scissorCount = 1};
		vk::PipelineRasterizationStateCreateInfo rasterizer{.depthClampEnable	= vk::False,
															.rasterizerDiscardEnable = vk::False,
															.polygonMode = vk::PolygonMode::eFill,
															.cullMode = vk::CullModeFlagBits::eBack,
															.frontFace = vk::FrontFace::eClockwise,
															.depthBiasEnable = vk::False,
															.lineWidth = 1.0f

		};

		vk::PipelineMultisampleStateCreateInfo multisampling {.rasterizationSamples = msaaSamples, .sampleShadingEnable = vk::False};
		//vk::PipelineMultisampleStateCreateInfo multisampling {.rasterizationSamples = msaaSamples, .sampleShadingEnable = vk::True, .minSampleShading = 0.2f};

		vk::PipelineColorBlendAttachmentState colorBlendAttachment{
			.blendEnable = vk::False,
			.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
		};
		
		vk::PipelineColorBlendStateCreateInfo colorBlending { 
			.logicOpEnable = vk::False,
			.logicOp = vk::LogicOp::eCopy,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment
		};

		vk::PipelineLayoutCreateInfo pipeLayoutInfo{.setLayoutCount = 0, .pushConstantRangeCount = 0};
		/*vk::PipelineLayoutCreateInfo pipeLayoutInfo {
			.setLayoutCount = 1,
			.pSetLayouts = &*descriptorSetLayout,
			.pushConstantRangeCount = 0
		};*/

		pipelineLayout = vkPlat -> CreatePipelineLayout(pipeLayoutInfo);

		vk::PipelineDepthStencilStateCreateInfo depthStencil {
			.depthTestEnable = vk::True,
			.depthWriteEnable = vk::True,
			.depthCompareOp = vk::CompareOp::eLess,
			.depthBoundsTestEnable = vk::False,
			.stencilTestEnable = vk::False
		};

		vk::Format depthFormat = vkPlat -> findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
		vk::Format swapChainFormat = vk::Format::eB8G8R8A8Srgb;

        vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
		    {.stageCount          = 2,
		     .pStages             = shaderStages,
		     .pVertexInputState   = &vertexInputInfo,
		     .pInputAssemblyState = &inputAssembly,
		     .pViewportState      = &viewportState,
		     .pRasterizationState = &rasterizer,
		     .pMultisampleState   = &multisampling,
		     .pColorBlendState    = &colorBlending,
		     .pDynamicState       = &dynamicState,
		     .layout              = pipelineLayout,
		     .renderPass          = nullptr},
		    {.colorAttachmentCount = 1, .pColorAttachmentFormats = &swapChainFormat}};

		vkPipeline = vkPlat -> CreatePipeline(pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
        return true;
    }

    void Pipeline::Unload() {

    }
}