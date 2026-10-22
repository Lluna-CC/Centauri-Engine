#include "vulkanPlatform.h"
#include "../Render/vulkanUtils.h"
#include <map>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<char const*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

namespace Centauri {

    VulkanPlatform::~VulkanPlatform() {

    }

	VulkanPlatform::VulkanPlatform(VulkanSurface* surf) {
		this -> surf = surf;
		InitializePlatform();
	}

	void VulkanPlatform::SetFramesInFlight(uint frames_in_flight) {
		CreateSyncObjects(frames_in_flight);
	}
	
	std::vector<vk::raii::CommandBuffer> VulkanPlatform::CreateCommandBuffers(uint count) {
		vk::CommandBufferAllocateInfo gAllocInfo{
			.commandPool = graphicsCommandPool,
			.level = vk::CommandBufferLevel::ePrimary, 
			.commandBufferCount = count
		};

		return std::move(vk::raii::CommandBuffers(dev,gAllocInfo));
	}
	
	std::vector<const char*> VulkanPlatform::RequiredInstanceExtensions() {
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back("VK_EXT_debug_utils");
		}

		return extensions;
	}

    void VulkanPlatform::InitializePlatform() {
        CreateInstance();
        SetupDebugMessenger();

		VkSurfaceKHR vkSurf = surf -> GetVulkanSurface(*inst);
		surface = vk::raii::SurfaceKHR(inst, vkSurf);
        
		
		ChoosePhysicalDevice();
		CreateLogicalDevice();
		
		CreateCommandPools();
    }

    void VulkanPlatform::CreateInstance() {
        constexpr vk::ApplicationInfo appInfo{.pApplicationName	= "Centauri Engine",
											  .applicationVersion =	VK_MAKE_VERSION(1,0,0),
											  .pEngineName = "No engine",
											  .engineVersion = VK_MAKE_VERSION(1,0,0),
											  .apiVersion = vk::ApiVersion14};
		
		auto requiredExtensions = RequiredInstanceExtensions();
		auto extensionProperties = context.enumerateInstanceExtensionProperties();
		
		auto unsupportedExtensionIt = 
			std::ranges::find_if(requiredExtensions, [&extensionProperties](auto const &requiredExtension)
													 {return std::ranges::none_of(extensionProperties, [&requiredExtension](auto const &extensionProperty)
																										{return strcmp(extensionProperty.extensionName, requiredExtension) == 0;});
													  });

	    if (unsupportedExtensionIt != requiredExtensions.end()) {
			throw std::runtime_error("Required extension not supported: " + std::string(*unsupportedExtensionIt));
		}

		std::vector<char const*> requiredLayers;
		if (enableValidationLayers) {
			requiredLayers.assign(validationLayers.begin(), validationLayers.end());
		}

		auto layerProperties = context.enumerateInstanceLayerProperties();
		auto unsupportedLayerIt = std::ranges::find_if(requiredLayers, 
														[&layerProperties](auto const &requiredLayer) {return std::ranges::none_of(layerProperties, 
																										[&requiredLayer](auto const &layerProperty)
																										{return strcmp(layerProperty.layerName, requiredLayer) == 0;}); 	
														});

		if (unsupportedLayerIt != requiredLayers.end()) {
			throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
		}

		vk::InstanceCreateInfo createInfo{
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
			.ppEnabledLayerNames = requiredLayers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data(),
		};

		inst = vk::raii::Instance(context, createInfo);
    }

	void VulkanPlatform::SetupDebugMessenger() {
				
		if (!enableValidationLayers) return;

		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );

		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
															vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | 
															vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags,
                                                                          .messageType     = messageTypeFlags,
                                                                          .pfnUserCallback = &debugCallback};
																	
		debugMessenger = inst.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
	}

    void VulkanPlatform::ChoosePhysicalDevice() {
		auto physicalDevices = inst.enumeratePhysicalDevices();

		if (physicalDevices.empty()) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::multimap<int, vk::raii::PhysicalDevice> candidates;
		for (const auto &pd : physicalDevices) {
			auto deviceProperties = pd.getProperties();
			auto deviceFeatures = pd.getFeatures();
			
			auto queueFamilies = pd.getQueueFamilyProperties();
			bool supportsGraphics = std::ranges::any_of(queueFamilies, [](auto const &qfp){return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });
			bool supportsTransfer = std::ranges::any_of(queueFamilies, [](auto const &qfp){return !!(qfp.queueFlags & vk::QueueFlagBits::eTransfer); });
			
			std::vector<const char*> requiredDeviceExtensions = {vk::KHRSwapchainExtensionName};

			auto availableDeviceExtensions = pd.enumerateDeviceExtensionProperties();
			bool supportsAllRequiredExtensions = std::ranges::all_of(requiredDeviceExtensions, 
																	[&availableDeviceExtensions](auto const &requiredExtension) 
																	{return std::ranges::any_of(availableDeviceExtensions, [&requiredExtension](auto const &availableExtension)
																	{return strcmp(availableExtension.extensionName, requiredExtension) == 0;});
																});


			auto features    = pd.template getFeatures2<vk::PhysicalDeviceFeatures2,
                                                        			vk::PhysicalDeviceVulkan11Features,
                                                                	vk::PhysicalDeviceVulkan13Features,
                                                                	vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

			bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
										    features.template get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy &&
											features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
											features.template get<vk::PhysicalDeviceVulkan13Features>().synchronization2 &&
											features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

			if (!deviceFeatures.geometryShader || deviceProperties.apiVersion < vk::ApiVersion13 || !supportsGraphics 
				|| !supportsAllRequiredExtensions || !supportsRequiredFeatures || !supportsTransfer)
			{
				continue;
			}

			uint32_t score = 0;

			if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
				score += 1000;
			}

			score += deviceProperties.limits.maxImageDimension2D;

			candidates.insert(std::make_pair(score, pd));
		}

		if (!candidates.empty() && candidates.rbegin() -> first > 0) {
			physicalDev = candidates.rbegin() -> second;
			//msaaSamples = getMaxUsableSampleCount();
			//std::cout << "Name of the selected device: " << physicalDev.getProperties().deviceName << std::endl;
			
		}
		else 
		{
			throw std::runtime_error("failed to find a suitable GPU");
		}
    }

    void VulkanPlatform::CreateLogicalDevice() {
		std::vector<vk::QueueFamilyProperties> queueFamilyProps = physicalDev.getQueueFamilyProperties();
		for (uint i = 0; i < queueFamilyProps.size(); ++i) {
			if ((queueFamilyProps[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDev.getSurfaceSupportKHR(i, *surface)) {
				graphicsQueueIndex = i;
				break;
			}
		}
		if (graphicsQueueIndex == ~0) {
			throw std::runtime_error("Could not find a queue for graphics and present");
		}

		float queuePriority = 0.5f;
		
		vk::DeviceQueueCreateInfo deviceQCreateInfos[1] = {
 		{.queueFamilyIndex = graphicsQueueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority}
		};

		vk::StructureChain<vk::PhysicalDeviceFeatures2,
							vk::PhysicalDeviceVulkan11Features,
							vk::PhysicalDeviceVulkan13Features,
							vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>

							featureChain {
								{.features = {.sampleRateShading = vk::True, .samplerAnisotropy = true}},
								{.shaderDrawParameters = true},
								{.synchronization2 = true, .dynamicRendering = true},
								{.extendedDynamicState = true}
							};

		std::vector<const char*> requiredDeviceExtension = {vk::KHRSwapchainExtensionName};
		
		vk::DeviceCreateInfo devCreateInf {
			.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = deviceQCreateInfos,
			.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
			.ppEnabledExtensionNames = requiredDeviceExtension.data()
		};

		dev = vk::raii::Device(physicalDev, devCreateInf);
		
		graphicsQueue = vk::raii::Queue(dev, graphicsQueueIndex, 0);
	}

	vk::PresentModeKHR ChoosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresents) {
		assert(std::ranges::any_of(availablePresents, [](const auto present) {return present == vk::PresentModeKHR::eFifo;}));
		
		//This could be a parameter somewhere
		bool use_mailBox = false;
		
		if (use_mailBox) {
			return std::ranges::any_of(availablePresents, [](const auto present) {return present == vk::PresentModeKHR::eMailbox;}) ?
					vk::PresentModeKHR::eMailbox :
					vk::PresentModeKHR::eFifo;
		}
		
		return vk::PresentModeKHR::eFifo;
	}

    void VulkanPlatform::CreateSwapChain(int width, int height, vk::raii::SwapchainKHR& swapChain, vk::Extent2D& swapChainExtent, 
                              vk::SurfaceFormatKHR& swapChainSurfaceFormat) {
        vk::SurfaceCapabilitiesKHR capabilities = physicalDev.getSurfaceCapabilitiesKHR(*surface);
		std::vector<vk::SurfaceFormatKHR> availableFormats = physicalDev.getSurfaceFormatsKHR(*surface);
		std::vector<vk::PresentModeKHR> availablePresents = physicalDev.getSurfacePresentModesKHR(*surface);

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			swapChainExtent = capabilities.currentExtent;
		}
        
		swapChainExtent = vk::Extent2D {
			std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};

        auto minImageCount = std::max(3u, capabilities.minImageCount);
		if ((0 < capabilities.maxImageCount) && (capabilities.maxImageCount < minImageCount)) {
			minImageCount = capabilities.maxImageCount;
		}

        const auto formatIt = std::ranges::find_if (availableFormats, [](const auto &format) {return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;}); 
		if (formatIt != availableFormats.end()) swapChainSurfaceFormat = *formatIt;
		else swapChainSurfaceFormat = availableFormats[0];

        vk::PresentModeKHR present = ChoosePresentMode(availablePresents);

		
		vk::SwapchainCreateInfoKHR swapChainCreateInfo{
			.surface = *surface,
			.minImageCount = minImageCount,
			.imageFormat = swapChainSurfaceFormat.format,
			.imageColorSpace = swapChainSurfaceFormat.colorSpace,
			.imageExtent = swapChainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = capabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = present,
			.clipped = true,
			.oldSwapchain = nullptr

		};

		// I have no clue of what to do here
		
		swapChain = vk::raii::SwapchainKHR(dev, swapChainCreateInfo);
	
    }
	
	void VulkanPlatform::CreateCommandPools() {
		vk::CommandPoolCreateInfo gPoolInfo {
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = graphicsQueueIndex
		};
		graphicsCommandPool = vk::raii::CommandPool(dev, gPoolInfo);
	}
	
	RenderPlatform* RenderPlatform::CreateRenderPlatform(Surface* surf) {
        return new VulkanPlatform(dynamic_cast<VulkanSurface*> (surf));
    }

	void VulkanPlatform::CreateSyncObjects(uint frames_in_flight) {
		assert(inFlightFences.empty());
		for (uint i = 0; i < frames_in_flight; ++i) {
			inFlightFences.emplace_back(dev, vk::FenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled});
		}
	}


    vk::raii::ImageView VulkanPlatform::CreateImageView(const vk::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t levels) {
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

	std::pair<vk::raii::Image, vk::raii::DeviceMemory> VulkanPlatform::CreateImage(
		uint32_t width, uint32_t height, uint32_t levels, vk::SampleCountFlagBits numSamples, vk::Format format, 
		vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties
	) {
		
		vk::ImageCreateInfo imageInfo {
			.imageType = vk::ImageType::e2D,
			.format = format,
			.extent = {width, height, 1},
			.mipLevels = levels,
			.arrayLayers = 1,
			.samples = numSamples,
			.tiling = tiling, 
			.usage = usage,
			.sharingMode = vk::SharingMode::eExclusive,
		};

		vk::raii::Image image = vk::raii::Image(dev, imageInfo);
	
		vk::MemoryRequirements memImageRequirements = image.getMemoryRequirements();
		vk::MemoryAllocateInfo allocInfo {
			.allocationSize = memImageRequirements.size, 
			.memoryTypeIndex = findMemoryType(memImageRequirements.memoryTypeBits, properties, physicalDev.getMemoryProperties())
		};

		vk::raii::DeviceMemory imageMemory = vk::raii::DeviceMemory(dev, allocInfo);
		image.bindMemory(imageMemory, 0);

		return {std::move(image), std::move(imageMemory)};
	}

	vk::Format VulkanPlatform::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const {
		for (vk::Format format : candidates) {
			vk::FormatProperties props = physicalDev.getFormatProperties(format);

			if (((tiling == vk::ImageTiling::eLinear) && ((props.linearTilingFeatures & features) == features)) ||
				((tiling == vk::ImageTiling::eOptimal) && ((props.optimalTilingFeatures & features) == features))) 
					return format;
			
			
		}
		throw std::runtime_error("failed to find supported format");
	}

	vk::raii::ShaderModule VulkanPlatform::createShaderModule(const std::vector<char>& code) const {
		vk::ShaderModuleCreateInfo createInfo{.codeSize = code.size() * sizeof(char), .pCode = reinterpret_cast<const uint32_t*>(code.data())};
		vk::raii::ShaderModule shaderModule{dev, createInfo};

		return shaderModule;
	}

	void VulkanPlatform::WaitForFence(uint fenceIdx) {
		auto fenceResult = dev.waitForFences(*inFlightFences[fenceIdx], vk::True, UINT64_MAX);
		if (fenceResult != vk::Result::eSuccess) {
			throw std::runtime_error("failed to wait for fence!");
		}
	}

	void VulkanPlatform::ResetFence(uint fenceIdx) {
		dev.resetFences(*inFlightFences[fenceIdx]);
	}

	void VulkanPlatform::SubmitQueue(const vk::SubmitInfo& submitInfo, size_t frameIdx) {
		graphicsQueue.submit(submitInfo, *inFlightFences[frameIdx]);
	}

	vk::Result VulkanPlatform::Present(const vk::PresentInfoKHR& presentInfo) {
		return graphicsQueue.presentKHR(presentInfo);
	}

	void VulkanPlatform::WaitDevice() {
		dev.waitIdle();
	}
}