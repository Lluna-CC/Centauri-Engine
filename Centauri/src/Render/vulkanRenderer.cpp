#include "vulkanRenderer.h"
#include "vulkanUtils.h"

namespace Centauri {

    VulkanRenderer::VulkanRenderer(VulkanPlatform* plat, ResourceManager* resMngr) {
        platform = plat;
		resourceManager = resMngr;

		InitializeRenderer();
	}

    void VulkanRenderer::InitializeRenderer() {
	
		graphicsCommandBuffers = platform -> CreateCommandBuffers(MAX_FRAMES_IN_FLIGHT);
    
		assert(presentCompleteSemaphores.empty());
		for (uint i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			presentCompleteSemaphores.push_back(platform -> CreateSemaphore(vk::SemaphoreCreateInfo()));
		}

		platform -> SetFramesInFlight(MAX_FRAMES_IN_FLIGHT);

		InitializeWindow(platform -> GetWidth(), platform -> GetHeight());
	}

	void VulkanRenderer::InitializeWindow(int width, int height) {
		
		swapchain = resourceManager -> Load<Swapchain>("main_swapchain", platform, width, height);

		colorAttachment = resourceManager -> Load<RenderAttachment>(
			"Color_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
			swapchain -> GetFormat().format, vk::ImageAspectFlagBits::eColor 
		);
		

		vk::Format depthFormat = platform -> findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
		depthAttachment = resourceManager -> Load<RenderAttachment>(
			"Depth_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eDepthStencilAttachment,
			depthFormat, vk::ImageAspectFlagBits::eDepth
		);
		

		renderFinishedSemaphores.clear();
		size_t imgCount = swapchain -> GetImageCount();
		
		for (uint i = 0; i < imgCount; ++i) {
			renderFinishedSemaphores.push_back(platform -> CreateSemaphore(vk::SemaphoreCreateInfo()));
		}
		

	}

	void VulkanRenderer::WindowResize(int width, int height) {
		
		swapchain -> CleanSwapchain();
		swapchain -> Reload(width, height);
		
		resourceManager -> Release<RenderAttachment>("Color_Attachment");
		colorAttachment = resourceManager -> Load<RenderAttachment>(
			"Color_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
			swapchain -> GetFormat().format, vk::ImageAspectFlagBits::eColor 
		);
		
		resourceManager -> Release<RenderAttachment>("Depth_Attachment");
		vk::Format depthFormat = platform -> findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
		depthAttachment = resourceManager -> Load<RenderAttachment>(
			"Depth_Attachment", platform, width, height,
			vk::ImageUsageFlagBits::eDepthStencilAttachment,
			depthFormat, vk::ImageAspectFlagBits::eDepth
		);
		
		renderFinishedSemaphores.clear();
		size_t imgCount = swapchain -> GetImageCount();
		
		for (uint i = 0; i < imgCount; ++i) {
			renderFinishedSemaphores.push_back(platform -> CreateSemaphore(vk::SemaphoreCreateInfo()));
		}
		

	}

    
    void VulkanRenderer::StartRendering() {
		platform -> WaitForFence(frameIdx);
		
		auto [res, img] = swapchain -> AcquireNextImage(presentCompleteSemaphores[frameIdx]);
		imageIdx = img;
		
		if (res == vk::Result::eErrorOutOfDateKHR) {
			platform -> WaitDevice();
            WindowResize(platform -> GetWidth(), platform -> GetHeight());
		}

		if (res != vk::Result::eSuccess && res != vk::Result::eSuboptimalKHR) {
			assert(res == vk::Result::eTimeout || res == vk::Result::eNotReady);
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		platform -> ResetFence(frameIdx);
		
		graphicsCommandBuffers[frameIdx].begin({});
		
		transition_image_layout(
			swapchain -> GetImage(imageIdx),
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eColorAttachmentOptimal,
			{},
			vk::AccessFlagBits2::eColorAttachmentWrite,
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::ImageAspectFlagBits::eColor,
			graphicsCommandBuffers[frameIdx]
		);

		transition_image_layout(
			depthAttachment -> GetImage(),
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthAttachmentOptimal,
			vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
			vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
			vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
			vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
			vk::ImageAspectFlagBits::eDepth,
			graphicsCommandBuffers[frameIdx]
		);

		transition_image_layout(
			colorAttachment -> GetImage(),
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eColorAttachmentOptimal,
			vk::AccessFlagBits2::eColorAttachmentWrite,
			vk::AccessFlagBits2::eColorAttachmentWrite,
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::ImageAspectFlagBits::eColor,
			graphicsCommandBuffers[frameIdx]
		);
		
		vk::ClearValue clearColor = vk::ClearColorValue(0.0f,0.0f,0.0f, 1.0f);
		vk::ClearValue clearDepth = vk::ClearDepthStencilValue(1.0f, 0.0f);

		vk::ResolveModeFlagBits resolveFlags = vk::ResolveModeFlagBits::eNone;
		if (msaaSamples != vk::SampleCountFlagBits::e1) resolveFlags = vk::ResolveModeFlagBits::eAverage;

		vk::RenderingAttachmentInfo colorAttachmentInfo = {
		    .imageView   = swapchain -> GetImageView(imageIdx),
		    .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
		    .loadOp      = vk::AttachmentLoadOp::eClear,
		    .storeOp     = vk::AttachmentStoreOp::eStore,
		    .clearValue  = clearColor};
		
		/*vk::RenderingAttachmentInfo colorAttachmentInfo = {
			.imageView 		= colorAttachment -> GetImageView(),
			.imageLayout	= vk::ImageLayout::eColorAttachmentOptimal,
			.resolveMode 	= resolveFlags,
			.resolveImageView = swapchain -> GetImageView(imageIdx),
			.resolveImageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp			= vk::AttachmentLoadOp::eClear,
			.storeOp		= vk::AttachmentStoreOp::eStore,
			.clearValue		= clearColor
		};*/

		vk::RenderingAttachmentInfo depthAttachmentInfo = {
			.imageView = depthAttachment -> GetImageView(),
			.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
			.loadOp = vk::AttachmentLoadOp::eClear,
			.storeOp = vk::AttachmentStoreOp::eDontCare,
			.clearValue = clearDepth
		};

		vk::RenderingInfo renderingInfo = {
			.renderArea				= {.offset = {0,0}, .extent = swapchain -> GetExtent()},
			.layerCount				= 1,
			.colorAttachmentCount 	= 1,
			.pColorAttachments 		= &colorAttachmentInfo,
			//.pDepthAttachment = &depthAttachmentInfo
		};
		
		graphicsCommandBuffers[frameIdx].beginRendering(renderingInfo);

		renderStarted = true;
	}
    
	void VulkanRenderer::FinishRendering() {
		if (!renderStarted) return;
		
		graphicsCommandBuffers[frameIdx].endRendering();
		
		transition_image_layout(
			swapchain -> GetImage(imageIdx),
			vk::ImageLayout::eColorAttachmentOptimal,
			vk::ImageLayout::ePresentSrcKHR,
			vk::AccessFlagBits2::eColorAttachmentWrite,
			{},
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::PipelineStageFlagBits2::eBottomOfPipe,
			vk::ImageAspectFlagBits::eColor,
			graphicsCommandBuffers[frameIdx]
		);

		graphicsCommandBuffers[frameIdx].end();

		//Present command buffer
		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		const vk::SubmitInfo submitInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores	= &*presentCompleteSemaphores[frameIdx],
			.pWaitDstStageMask	= &waitDestinationStageMask,
			.commandBufferCount = 1,
			.pCommandBuffers	= &*graphicsCommandBuffers[frameIdx],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores 	= &*renderFinishedSemaphores[imageIdx]
		};

		platform -> SubmitQueue(submitInfo, frameIdx);

		//Present swapchain
		const vk::PresentInfoKHR presentInfoKHR {
			.waitSemaphoreCount	= 1,
			.pWaitSemaphores	= &*renderFinishedSemaphores[imageIdx],
			.swapchainCount		= 1,
			.pSwapchains		= &*swapchain -> GetSwapchain(),
			.pImageIndices		= &imageIdx
		}; 
		
		vk::Result result = platform -> Present(presentInfoKHR);
		renderStarted = false;

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR  || framebufferResized) {
			platform -> WaitDevice();
            WindowResize(platform -> GetWidth(), platform -> GetHeight());
		}

		else {
			assert(result == vk::Result::eSuccess);
		}

		frameIdx = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
		
	}
	
	void VulkanRenderer::BindPipeline(ResourceHandle<Pipeline> pipeline) {
		graphicsCommandBuffers[frameIdx].bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline -> GetPipeline());

		vk::Extent2D extent = swapchain -> GetExtent();
		graphicsCommandBuffers[frameIdx].setViewport(0, vk::Viewport(0.0f,0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f));
		graphicsCommandBuffers[frameIdx].setScissor(0, vk::Rect2D(vk::Offset2D(0,0), extent));
	}

	void VulkanRenderer::SillyRender() {
		graphicsCommandBuffers[frameIdx].draw(3, 1, 0, 0);
	}

	Renderer* Renderer::CreateRenderer(RenderPlatform* plat, ResourceManager* resMngr) {

        return new VulkanRenderer(dynamic_cast<VulkanPlatform*> (plat), resMngr);
    }

	
}