#include "vulkanPlatform.h"
#include <map>

namespace Centauri {

    VulkanPlatform::~VulkanPlatform() {

    }

	VulkanPlatform::VulkanPlatform(const VulkanSurface& surf) {
		InitializePlatform(surf);
	}

	std::vector<const char*> VulkanPlatform::RequiredInstanceExtensions() {
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}

    void VulkanPlatform::InitializePlatform(const VulkanSurface& surf) {
        CreateInstance();
        
		VkSurfaceKHR vkSurf = surf.GetVulkanSurface(*inst);
		surface = vk::raii::SurfaceKHR(inst, vkSurf);
        
		ChoosePhysicalDevice();
		CreateLogicalDevice();
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

		/*if (enableValidationLayers) {
			requiredLayers.assign(validationLayers.begin(), validationLayers.end());
		}*/

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
			.queueCreateInfoCount = 2,
			.pQueueCreateInfos = deviceQCreateInfos,
			.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
			.ppEnabledExtensionNames = requiredDeviceExtension.data()
		};

		dev = vk::raii::Device(physicalDev, devCreateInf);
		graphicsQueue = vk::raii::Queue(dev, graphicsQueueIndex, 0);
    }

    void VulkanPlatform::createSwapChain(int width, int height) {
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

        vk::PresentModeKHR present;
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

		swapChain = vk::raii::SwapchainKHR(dev, swapChainCreateInfo);
	
    }
	
	
	RenderPlatform* RenderPlatform::CreateRenderPlatform(const VulkanSurface& surf) {
        return new VulkanPlatform(surf);
    }


        
}