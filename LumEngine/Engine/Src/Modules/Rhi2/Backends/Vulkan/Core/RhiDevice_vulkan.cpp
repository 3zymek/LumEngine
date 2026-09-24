#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi2/RhiAdapter.hpp"
#include "Platform/SurfaceProvider.hpp"
#include "Platform/VulkanSurfaceProvider.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum::rhi::vk {

	void VulkanDevice::Initialize( const RenderDeviceCreateInfo& info ) noexcept {

		if (volkInitialize( ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to initialize volk (Vulkan loader)" );
		}

		m_SurfaceProvider = static_cast<IVulkanSurfaceProvider*>(&info.m_SurfaceProvider( ));

		create_vk_instance( info );
		volkLoadInstance( m_Instance );
		create_main_surface( );
		choose_adapter( );
		create_logical_device( );
		acquire_queues( );
		create_shader_stages( );
		create_swapchain( {} );
		extract_swapchain_images( );
		create_main_pipeline( );
		create_command_pool( );
		allocate_command_buffers( );
		create_sync_primitives( );

	}

	void VulkanDevice::Finalize( ) noexcept {

		if (m_LogicalDevice != VK_NULL_HANDLE) {

			vkDeviceWaitIdle( m_LogicalDevice );

			vkDestroyFence( m_LogicalDevice, m_Fence, nullptr );

			for (auto& semaphore : m_ImageAvailableSemaphores) {
				vkDestroySemaphore( m_LogicalDevice, semaphore, nullptr );
			}
			for (auto& semaphore : m_RenderFinishedSemaphores) {
				vkDestroySemaphore( m_LogicalDevice, semaphore, nullptr );
			}

			vkDestroyCommandPool( m_LogicalDevice, m_CmdPool, nullptr );

			vkDestroyPipeline( m_LogicalDevice, m_MainPipeline, nullptr );
			vkDestroyPipelineLayout( m_LogicalDevice, m_PipelineLayout, nullptr );
			vkDestroyShaderModule( m_LogicalDevice, DT_Vertex, nullptr );
			vkDestroyShaderModule( m_LogicalDevice, DT_Fragment, nullptr );

			for (auto view : m_SwapchainImageViews) {
				vkDestroyImageView( m_LogicalDevice, view, nullptr );
			}
			for (auto img : m_SwapchainImages) {
				vkDestroyImage( m_LogicalDevice, img, nullptr );
			}

			vkDestroySwapchainKHR( m_LogicalDevice, m_Swapchain, nullptr );

			vkDestroySurfaceKHR( m_Instance, m_MainSurface, nullptr );
			vkDestroyDevice( m_LogicalDevice, nullptr );
			vkDestroyInstance( m_Instance, nullptr );

		}

	}

	void VulkanDevice::DrawFrame( ) noexcept {

		// Sync GPU and CPU
		vkWaitForFences( m_LogicalDevice, 1, &m_Fence, VK_TRUE, UINT64_MAX );
		vkResetFences( m_LogicalDevice, 1, &m_Fence );

		// Acquire image from swapchain
		uint32 imageIndex{};

		VkSemaphore& imageAvailableSemaphore = m_ImageAvailableSemaphores[ m_CurrentFrame ];

		vkAcquireNextImageKHR( m_LogicalDevice, m_Swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex );

		// Record commands
		VkCommandBuffer& buffer = m_CmdBuffers[ 0 ];
		vkResetCommandBuffer( buffer, 0 );

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer( buffer, &beginInfo );

		// Entry barrier
		VkImageMemoryBarrier2 barrierToRender{};
		barrierToRender.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
		barrierToRender.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrierToRender.srcAccessMask = VK_ACCESS_2_NONE;
		barrierToRender.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrierToRender.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		barrierToRender.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrierToRender.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrierToRender.image = m_SwapchainImages[ imageIndex ];
		barrierToRender.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

		VkDependencyInfo dependencyInfo{};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependencyInfo.imageMemoryBarrierCount = 1;
		dependencyInfo.pImageMemoryBarriers = &barrierToRender;

		vkCmdPipelineBarrier2( buffer, &dependencyInfo );

		// DYNAMIC RENDERING
		VkRenderingAttachmentInfo colorAttachment{};
		colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachment.imageView = m_SwapchainImageViews[ imageIndex ];
		colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.clearValue = { {{ 0.0f, 0.0f, 0.0f, 0.0f }} };

		VkExtent2D currentExtent = m_Adapter.m_SurfaceSupport.m_Capabilities.currentExtent;

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.renderArea = { {0, 0}, currentExtent };
		renderingInfo.layerCount = 1;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments = &colorAttachment;

		vkCmdBeginRendering( buffer, &renderingInfo );

		vkCmdBindPipeline( buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_MainPipeline );

		VkViewport viewport{ 0.0f, 0.0f, (float32) currentExtent.width, (float32) currentExtent.height, 0.0f, 1.0f };
		VkRect2D scissor{ {0, 0}, currentExtent };
		vkCmdSetViewport( buffer, 0, 1, &viewport );
		vkCmdSetScissor( buffer, 0, 1, &scissor );
		
		vkCmdDraw( buffer, 3, 1, 0, 0 ); // Draw simple triangle

		vkCmdEndRendering( buffer );

		// Outry barrier
		VkImageMemoryBarrier2 barrierToPresent = barrierToRender;
		barrierToPresent.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		barrierToPresent.dstAccessMask = 0;
		barrierToPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrierToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrierToPresent.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;

		dependencyInfo.pImageMemoryBarriers = &barrierToPresent;
		vkCmdPipelineBarrier2( buffer, &dependencyInfo );


		// Send commands to GPU
		VkSemaphoreSubmitInfo waitSemaphoreInfo{};
		waitSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		waitSemaphoreInfo.semaphore = imageAvailableSemaphore;
		waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSemaphoreSubmitInfo signalSemaphoreInfo{};
		signalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		signalSemaphoreInfo.semaphore = m_RenderFinishedSemaphores[imageIndex];
		signalSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;

		VkCommandBufferSubmitInfo cmdBufferInfo{};
		cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
		cmdBufferInfo.commandBuffer = buffer;

		VkSubmitInfo2 submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.waitSemaphoreInfoCount = 1;
		submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
		submitInfo.signalSemaphoreInfoCount = 1;
		submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;
		submitInfo.commandBufferInfoCount = 1;
		submitInfo.pCommandBufferInfos = &cmdBufferInfo;

		vkEndCommandBuffer( buffer );
		vkQueueSubmit2( m_GraphicsQueue, 1, &submitInfo, m_Fence );

		// Present
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[ imageIndex ];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR( m_PresentQueue, &presentInfo );

		m_CurrentFrame = (m_CurrentFrame + 1) % LUM_MAX_FRAMES_IN_FLIGHT;

	}

	void VulkanDevice::create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept {

		std::vector<const char*> layers = {
			"VK_LAYER_KHRONOS_validation"
		};

		std::vector<const char*> extensions = info.m_InstanceExtensions;
		if (info.m_EnableValidation) {
			extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		}

		//uint32 apiVersion = 0;
		//vkEnumerateInstanceVersion( &apiVersion );

		//uint32 apiMajor = VK_API_VERSION_MAJOR( apiVersion );
		//uint32 apiMinor = VK_API_VERSION_MINOR( apiVersion );
		//uint32 apiPatch = VK_API_VERSION_PATCH( apiVersion );

		VkApplicationInfo appInfo{};
		//appInfo.apiVersion = VK_MAKE_VERSION( apiMajor, apiMinor, apiPatch );
		appInfo.apiVersion = VK_MAKE_VERSION( 1, 3, 0 );
		appInfo.engineVersion = VK_MAKE_VERSION( 0, 3, 0 );
		appInfo.pApplicationName = "Blank";
		appInfo.pEngineName = "LumEngine";

		VkInstanceCreateInfo desc{};
		desc.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		desc.pApplicationInfo = &appInfo;
		desc.enabledExtensionCount = SafeCast<uint32>( extensions.size( ) );
		desc.ppEnabledExtensionNames = extensions.data( );
		desc.enabledLayerCount = layers.size( );
		desc.ppEnabledLayerNames = layers.data( );

		// ENABLE VALIDATION
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (info.m_EnableValidation) {

			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

			debugCreateInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

			debugCreateInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			debugCreateInfo.pfnUserCallback =
				[]( VkDebugUtilsMessageSeverityFlagBitsEXT severity,
					VkDebugUtilsMessageTypeFlagsEXT type,
					const VkDebugUtilsMessengerCallbackDataEXT* data,
					void* userData ) -> VkBool32 {
						if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
							LUM_LOG_ERROR( "[VULKAN VALIDATION] {}", data->pMessage );
						}
						else {
							LUM_LOG_WARN( "[VULKAN VALIDATION] {}", data->pMessage );
						}
						return VK_FALSE;
				};

			desc.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
			desc.enabledLayerCount = SafeCast<uint32>( layers.size( ) );
			desc.ppEnabledLayerNames = layers.data( );

		}

		if (vkCreateInstance( &desc, nullptr, &m_Instance ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create VkInstance!" );
		}

	}

	void VulkanDevice::choose_adapter( ) noexcept {

		uint32 numDevices = 0;
		vkEnumeratePhysicalDevices( m_Instance, &numDevices, nullptr );
		if (numDevices <= 0) {
			LUM_LOG_FATAL( "There are no physical devices to be found!" );
			return;
		}
		std::vector<VkPhysicalDevice> devices( numDevices );
		vkEnumeratePhysicalDevices( m_Instance, &numDevices, devices.data( ) );

		VulkanAdapter bestAdapter{};

		for (uint32 i = 0; i < numDevices; i++) {

			Result<VulkanAdapter> evaluated = m_AdapterEvaluator.EvaluateAdapter( devices[ i ], m_MainSurface );

			if (!evaluated)
				continue;

			auto& val = evaluated.ValueRef( );

			if (val.m_Score > bestAdapter.m_Score && val.m_Queues.IsValid( ))
				bestAdapter = val;

		}

		LUM_LOG_INFO( "Score is: {}", bestAdapter.m_Score );
		LUM_LOG_INFO( bestAdapter.m_Queues.IsValid( ) ? "queues are valid" : "queues are invalid" );

		if (bestAdapter.m_Score == 0 || !bestAdapter.m_Queues.IsValid( )) {
			LUM_LOG_FATAL( "Failed to initialize Vulkan backend: None of the ({}) detected GPU(s) satisfy LumEngine's minimum required features!", numDevices );
			return;
		}

		m_Adapter = bestAdapter;
		LUM_LOG_INFO( "{}", bestAdapter.m_Score );

	}

	void VulkanDevice::create_logical_device( ) noexcept {

		// ENABLE SWAPCHAIN CONFIGURATION
		std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		const auto& queues = m_Adapter.m_Queues;

		float32 priorities = 1.0f;

		VkDeviceQueueCreateInfo graphicsQueueInfo{};
		graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueInfo.pQueuePriorities = &priorities;
		graphicsQueueInfo.queueCount = 1;
		graphicsQueueInfo.queueFamilyIndex = queues.m_GraphicsQueueIndex;

		VkDeviceQueueCreateInfo computeQueueInfo{};
		computeQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueInfo.pQueuePriorities = &priorities;
		computeQueueInfo.queueCount = 1;
		computeQueueInfo.queueFamilyIndex = queues.m_ComputeQueueIndex;

		VkDeviceQueueCreateInfo presentQueueInfo{};
		presentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		presentQueueInfo.pQueuePriorities = &priorities;
		presentQueueInfo.queueCount = 1;
		presentQueueInfo.queueFamilyIndex = queues.m_PresentQueueIndex;

		std::vector<VkDeviceQueueCreateInfo> queueInfos{};
		queueInfos.push_back( graphicsQueueInfo );

		if (queues.m_GraphicsQueueIndex != queues.m_PresentQueueIndex) {
			queueInfos.push_back( presentQueueInfo );
		}

		if (queues.HasQueue( queues.m_ComputeQueueIndex ) &&
			 queues.m_ComputeQueueIndex != queues.m_PresentQueueIndex && 
			 queues.m_ComputeQueueIndex != queues.m_GraphicsQueueIndex) {

			queueInfos.push_back( computeQueueInfo );

		}

		VkDeviceCreateInfo info{ };
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.enabledExtensionCount = SafeCast<uint32>( deviceExtensions.size( ) );
		info.ppEnabledExtensionNames = deviceExtensions.data( );
		info.pEnabledFeatures = &m_Adapter.m_Features;
		info.queueCreateInfoCount = SafeCast<uint32>( queueInfos.size( ) );
		info.pQueueCreateInfos = queueInfos.data( );
		info.pNext = detail::LogicalDeviceFeatures::GetStatic( ).GetChainHead( );

		if (vkCreateDevice( m_Adapter.m_Device, &info, nullptr, &m_LogicalDevice ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create Logical Device! (Vulkan)" );
		}

	}

	void VulkanDevice::acquire_queues( ) noexcept {

		vkGetDeviceQueue( m_LogicalDevice, m_Adapter.m_Queues.m_GraphicsQueueIndex, 0, &m_GraphicsQueue );
		vkGetDeviceQueue( m_LogicalDevice, m_Adapter.m_Queues.m_PresentQueueIndex, 0, &m_PresentQueue );

		if (m_Adapter.m_Queues.HasQueue( m_Adapter.m_Queues.m_ComputeQueueIndex )) {
			vkGetDeviceQueue( m_LogicalDevice, m_Adapter.m_Queues.m_ComputeQueueIndex, 0, &m_ComputeQueue );
		}

	}

	void VulkanDevice::create_shader_stages( ) noexcept {

		auto createShader = [ & ]( const Path& path, VkShaderModule& module ) -> void {

			auto code = FileSystem::ReadBinaryFile( path );
			if (!code) {
				LUM_LOG_FATAL( "Failed to read {} file: {}", path.ToString( ), code.GetError( ) );
				return;
			}

			VkShaderModuleCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			info.codeSize = code.ValueRef( ).size( ) * sizeof(uint32);
			info.pCode = code.ValueRef( ).data( );

			if (vkCreateShaderModule( m_LogicalDevice, &info, nullptr, &module ) != VK_SUCCESS) {
				LUM_LOG_FATAL( "Failed to create shader module from file '{}'! (Vulkan)", path.ToString( ) );
				return;
			}

		};

		createShader( 
			ResourceLoader::ResolveResourcePath( ResourceRoot::External, "debug.vert.spv" ), 
			DT_Vertex 
		);

		createShader(
			ResourceLoader::ResolveResourcePath( ResourceRoot::External, "debug.frag.spv" ),
			DT_Fragment
		);

	}

	void VulkanDevice::create_main_surface( ) noexcept {

		m_MainSurface = m_SurfaceProvider( ).CreateSurface( m_Instance );

	}

	void VulkanDevice::create_swapchain( TVector2<uint32> windowSize ) noexcept {

		VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		uint32 numQueueFamilyIndex = 0;
		const uint32* pQueueFamilyIndices = nullptr;

		uint32 graphicsIdx = m_Adapter.m_Queues.m_GraphicsQueueIndex;
		uint32 presentIdx = m_Adapter.m_Queues.m_PresentQueueIndex;

		uint32 queueFamilyIndices[ ] = { graphicsIdx, presentIdx };

		if (graphicsIdx != presentIdx) {
			sharingMode = VK_SHARING_MODE_CONCURRENT;
			numQueueFamilyIndex = 2;
			pQueueFamilyIndices = queueFamilyIndices;
		}

		const auto& capabilities = m_Adapter.m_SurfaceSupport.m_Capabilities;
		uint32 imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
			imageCount = capabilities.maxImageCount;
		}

		VkPresentModeKHR presentMode = m_Adapter.m_SurfaceSupport.SelectPresentMode( );
		VkSurfaceFormatKHR surfaceFormat = m_Adapter.m_SurfaceSupport.SelectSurfaceFormat( );
		VkSwapchainKHR oldSwapchain = m_Swapchain;
		VkExtent2D extent{};
		if (capabilities.currentExtent.width != UINT32_MAX) {
			extent = capabilities.currentExtent;
		}
		else {
			extent.width = Clamp(
				windowSize.m_X,
				capabilities.minImageExtent.width,
				capabilities.maxImageExtent.width
			);
			extent.height = Clamp(
				windowSize.m_Y,
				capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height
			);
		}

		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.pQueueFamilyIndices = pQueueFamilyIndices;
		info.queueFamilyIndexCount = numQueueFamilyIndex;
		info.clipped = VK_TRUE;
		info.presentMode = presentMode;
		info.oldSwapchain = oldSwapchain;

		info.minImageCount = imageCount;
		info.imageColorSpace = surfaceFormat.colorSpace;
		info.imageFormat = surfaceFormat.format;
		info.imageSharingMode = sharingMode;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		info.imageExtent = extent;
		info.imageArrayLayers = 1;

		info.preTransform = capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.surface = m_MainSurface;

		if (vkCreateSwapchainKHR( m_LogicalDevice, &info, nullptr, &m_Swapchain ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create Swapchain! (Vulkan)" );
			return;
		}
		if (oldSwapchain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR( m_LogicalDevice, oldSwapchain, nullptr );
		}

	}

	void VulkanDevice::extract_swapchain_images( ) noexcept {

		for (auto& view : m_SwapchainImageViews) {
			if (view != VK_NULL_HANDLE) {
				vkDestroyImageView( m_LogicalDevice, view, nullptr );
			}
		}

		uint32 numImages = 0;
		vkGetSwapchainImagesKHR( m_LogicalDevice, m_Swapchain, &numImages, nullptr );
		m_SwapchainImages.resize( numImages );
		vkGetSwapchainImagesKHR( m_LogicalDevice, m_Swapchain, &numImages, m_SwapchainImages.data( ) );

		VkSurfaceFormatKHR surfaceFormat = m_Adapter.m_SurfaceSupport.SelectSurfaceFormat( );

		m_SwapchainImageViews.resize( numImages );
		for (uint32 i = 0; i < numImages; i++) {

			VkImageViewCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.image = m_SwapchainImages[ i ];
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = surfaceFormat.format;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			if (vkCreateImageView( m_LogicalDevice, &info, nullptr, &m_SwapchainImageViews[ i ] ) != VK_SUCCESS) {
				LUM_LOG_FATAL( "Failed to create swapchain image view at index {}! (Vulkan)", i );
				return;
			}

		}

	}

	void VulkanDevice::create_main_pipeline( ) noexcept {

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = nullptr;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = nullptr;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = SafeCast<uint32>( dynamicStates.size( ) );
		dynamicStateInfo.pDynamicStates = dynamicStates.data( );

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationInfo.lineWidth = 1.0f;

		VkPipelineColorBlendStateCreateInfo colorBlend{};
		colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = 
			VK_COLOR_COMPONENT_R_BIT | 
			VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | 
			VK_COLOR_COMPONENT_A_BIT;

		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlend.attachmentCount = 1;
		colorBlend.pAttachments = &colorBlendAttachment;

		VkPipelineMultisampleStateCreateInfo msInfo{};
		msInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		msInfo.sampleShadingEnable = VK_FALSE;
		msInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkSurfaceFormatKHR surfaceFormat = m_Adapter.m_SurfaceSupport.SelectSurfaceFormat( );

		VkPipelineRenderingCreateInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachmentFormats = &surfaceFormat.format;

		VkPipelineShaderStageCreateInfo vertStageInfo{};
		vertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertStageInfo.module = DT_Vertex;
		vertStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragStageInfo{};
		fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragStageInfo.module = DT_Fragment;
		fragStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[ ] = { vertStageInfo, fragStageInfo };

		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 0;
		layoutInfo.pSetLayouts = nullptr;
		layoutInfo.pushConstantRangeCount = 0;
		layoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout( m_LogicalDevice, &layoutInfo, nullptr, &m_PipelineLayout ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create pipeline layout! (Vulkan)" );
			return;
		}

		VkGraphicsPipelineCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		info.pVertexInputState = &detail::DefaultVertexLayout::GetStatic( ).GetStateCreateInfo( );
		info.pViewportState = &viewportInfo;
		info.pInputAssemblyState = &inputAssemblyInfo;
		info.pRasterizationState = &rasterizationInfo;
		info.pColorBlendState = &colorBlend;
		info.pMultisampleState = &msInfo;
		info.pDynamicState = &dynamicStateInfo;
		info.pNext = &renderingInfo;
		info.pStages = shaderStages;
		info.stageCount = 2;
		info.layout = m_PipelineLayout;

		if (vkCreateGraphicsPipelines( m_LogicalDevice, VK_NULL_HANDLE, 1, &info, nullptr, &m_MainPipeline ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create main graphics pipeline! (Vulkan)" );
			return;
		}

	}

	void VulkanDevice::create_command_pool( ) noexcept {

		if (!m_Adapter.m_Queues.HasQueue( m_Adapter.m_Queues.m_GraphicsQueueIndex )) {
			LUM_LOG_ERROR( "Cannot create command pool: Adapter doesn't have graphics queue index! (Vulkan)" );
			return;
		}

		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = m_Adapter.m_Queues.m_GraphicsQueueIndex;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		if (vkCreateCommandPool( m_LogicalDevice, &info, nullptr, &m_CmdPool ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create command pool! (Vulkan)" );
			return;
		}

	}

	void VulkanDevice::allocate_command_buffers( ) noexcept {

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_CmdPool;
		info.commandBufferCount = SafeCast<uint32>( m_CmdBuffers.size( ) );
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		vkAllocateCommandBuffers( m_LogicalDevice, &info, m_CmdBuffers.data( ) );

	}

	void VulkanDevice::create_sync_primitives( ) noexcept {

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		usize numImages = m_SwapchainImages.size( );

		for (auto& semaphore : m_ImageAvailableSemaphores) {
			if (vkCreateSemaphore( m_LogicalDevice, &semaphoreInfo, nullptr, &semaphore ) != VK_SUCCESS) {
				LUM_LOG_FATAL( "Failed to create image available semaphore! (Vulkan)",  );
				return;
			}
		}

		m_RenderFinishedSemaphores.resize( numImages );
		for (auto& semaphore : m_RenderFinishedSemaphores) {
			if (vkCreateSemaphore( m_LogicalDevice, &semaphoreInfo, nullptr, &semaphore ) != VK_SUCCESS) {
				LUM_LOG_FATAL( "Failed to create render finished semaphore! (Vulkan)" );
				return;
			}
		}


		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateFence( m_LogicalDevice, &fenceCreateInfo, nullptr, &m_Fence ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create fence! (Vulkan)" );
			return;
		}

	}

} // namespace lum::rhi::vk