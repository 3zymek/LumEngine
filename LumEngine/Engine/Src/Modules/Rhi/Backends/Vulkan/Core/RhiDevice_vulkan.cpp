/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header impl file
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

#include "Core/Utils/Assert.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Platform/FileSystem/FileSystem.hpp"
#include "Platform/FileSystem/Path.hpp"
#include "Platform/VulkanSurfaceProvider.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiImage_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiTypes_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum::rhi::vk {

	// ===========================================================
	// PUBLIC
	// ===========================================================

	void VulkanDevice::Initialize( const RenderDeviceCreateInfo& info ) noexcept {

		if (volkInitialize( ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to initialize volk (Vulkan loader)" );
			return;
		}

		m_SurfaceProvider = static_cast<IVulkanSurfaceProvider*>(&info.m_SurfaceProvider( ));

		m_IsInitialized = true;

		bind_commands_implementations( );

		create_vk_instance( info );
		volkLoadInstance( m_Instance );
		create_main_surface( );
		choose_adapter( );
		create_logical_device( );
		acquire_queues( );
		recreate_swapchain( );
		recreate_swapchain_images( );
		create_command_pool( );
		allocate_command_buffers( );
		create_sync_primitives( );
		create_staging_buffers( );

		info.m_EventBus( ).SubscribePermanently<EWindowResized>(
			[ & ]( const EWindowResized& e ) {
				m_WindowSize = { e.m_Width, e.m_Height };
			}
		);

	}

	void VulkanDevice::Finalize( ) noexcept {

		assert_device( );

		if (m_LogicalDevice != VK_NULL_HANDLE && m_IsInitialized) {

			vkDeviceWaitIdle( m_LogicalDevice );

			for (auto&& [handle, buffer] : m_Buffers.Iterate( )) {
				DestroyBuffer( handle );
			}
			for (auto&& [handle, value] : m_Pipelines.Iterate( )) {
				DestroyPipeline( handle );
			}
			for (auto&& [handle, value] : m_Images.Iterate( )) {
				DestroyImage( handle );
			}
			for (auto&& [handle, value] : m_Samplers.Iterate( )) {
				DestroySampler( handle );
			}
			for (auto&& [handle, value] : m_ResourceSets.Iterate( )) {
				DestroyResourceSet( handle );
			}
			for (auto&& [handle, value] : m_ResourceLayouts.Iterate( )) {
				DestroyResourceLayout( handle );
			}
			for (auto& fence : m_InFlightFences) {
				vkDestroyFence( m_LogicalDevice, fence, nullptr );
			}

			for (auto& semaphore : m_ImageAvailableSemaphores) {
				vkDestroySemaphore( m_LogicalDevice, semaphore, nullptr );
			}
			for (auto& semaphore : m_RenderFinishedSemaphores) {
				vkDestroySemaphore( m_LogicalDevice, semaphore, nullptr );
			}

			vkDestroyCommandPool( m_LogicalDevice, m_CmdPool, nullptr );

			for (auto view : m_SwapchainImageViews) {
				vkDestroyImageView( m_LogicalDevice, view, nullptr );
			}

			vkDestroySwapchainKHR( m_LogicalDevice, m_Swapchain, nullptr );

			vkDestroySurfaceKHR( m_Instance, m_MainSurface, nullptr );
			vkDestroyDevice( m_LogicalDevice, nullptr );
			vkDestroyInstance( m_Instance, nullptr );

		}

	}

	void VulkanDevice::UpdateFrame( ) noexcept {

		assert_device( );

		// Sync GPU and CPU
		vkWaitForFences( m_LogicalDevice, 1, &m_InFlightFences[ m_CurrentFrame ], VK_TRUE, UINT64_MAX );

		auto& boundStagingBuffers = m_TemporaryStagingBuffers[ m_CurrentFrame ];
		while (!boundStagingBuffers.empty( )) {

			DestroyBuffer( boundStagingBuffers.front( ) );
			boundStagingBuffers.pop( );

		}

		// Acquire image from swapchain
		uint32 imageIndex{};
		VkSemaphore& imageAvailableSemaphore = m_ImageAvailableSemaphores[ m_CurrentFrame ];
		VkResult acquireImageResult = vkAcquireNextImageKHR( m_LogicalDevice, m_Swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex );

		if (acquireImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
			handle_resize( );
			return;
		}
		if (acquireImageResult != VK_SUCCESS && acquireImageResult != VK_SUBOPTIMAL_KHR) {
			return;
		}
		vkResetFences( m_LogicalDevice, 1, &m_InFlightFences[ m_CurrentFrame ] );





		// Begin recording
		VkCommandBuffer& commandBuffer = m_CmdBuffers[ m_CurrentFrame ];
		vkResetCommandBuffer( commandBuffer, 0 );

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer( commandBuffer, &beginInfo );


		process_commands<&CommandBuffer::GetPreRenderByteStream>( commandBuffer );

		 
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

		vkCmdPipelineBarrier2( commandBuffer, &dependencyInfo );





		// Dynamic Rendering
		VkRenderingAttachmentInfo colorAttachment{};
		colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachment.imageView = m_SwapchainImageViews[ imageIndex ];
		colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.clearValue = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.renderArea = { { 0, 0 }, m_SwapchainExtent };
		renderingInfo.layerCount = 1;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments = &colorAttachment;

		vkCmdBeginRendering( commandBuffer, &renderingInfo );

		process_commands<&CommandBuffer::GetRenderByteStream>( commandBuffer );

		vkCmdEndRendering( commandBuffer );







		// Outry barrier
		VkImageMemoryBarrier2 barrierToPresent = barrierToRender;
		barrierToPresent.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		barrierToPresent.dstAccessMask = 0;
		barrierToPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrierToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrierToPresent.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;

		dependencyInfo.pImageMemoryBarriers = &barrierToPresent;
		vkCmdPipelineBarrier2( commandBuffer, &dependencyInfo );




		// Send commands to GPU
		VkSemaphoreSubmitInfo waitSemaphoreInfo{};
		waitSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		waitSemaphoreInfo.semaphore = imageAvailableSemaphore;
		waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSemaphoreSubmitInfo signalSemaphoreInfo{};
		signalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		signalSemaphoreInfo.semaphore = m_RenderFinishedSemaphores[ imageIndex ];
		signalSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;

		VkCommandBufferSubmitInfo cmdBufferInfo{};
		cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
		cmdBufferInfo.commandBuffer = commandBuffer;

		VkSubmitInfo2 submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.waitSemaphoreInfoCount = 1;
		submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
		submitInfo.signalSemaphoreInfoCount = 1;
		submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;
		submitInfo.commandBufferInfoCount = 1;
		submitInfo.pCommandBufferInfos = &cmdBufferInfo;

		if (!LUM_VK_RESULT_CHECK( vkEndCommandBuffer( commandBuffer ) )) {
			return;
		}

		if (!LUM_VK_RESULT_CHECK( vkQueueSubmit2( m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[ m_CurrentFrame ] ) )) {
			return;
		}




		// Present
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[ imageIndex ];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR( m_PresentQueue, &presentInfo );




		// Calculate Frame
		m_CurrentFrame = (m_CurrentFrame + 1) % k_MaxFramesInFlight;
		m_CommandBuffersToFlush.clear( );

	}

	void VulkanDevice::SendCommandBuffer( CommandBuffer& buffer ) noexcept {

		assert_device( );
		m_CommandBuffersToFlush.push_back( buffer );

	}







	// ===========================================================
	// RUNTIME HELPERS (PRIVATE)
	// ===========================================================

	void VulkanDevice::assert_device( ) const {
		LUM_ASSERT( m_IsInitialized, "RHI Device is not initialized!" );
	}

	bool VulkanDevice::create_shader_module( const Path& path, VkShaderModule& module ) const noexcept {

		auto resBinaryCode = FileSystem::ReadBinaryFile( path );
		if (!resBinaryCode) {
			LUM_LOG_ERROR( "Failed to create shader: {}!", resBinaryCode.GetError( ) );
			return false;
		}
		auto& binary = resBinaryCode.ValueRef( );

		VkShaderModuleCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkInfo.pCode = binary.data( );
		vkInfo.codeSize = binary.size( ) / sizeof( uint32 );

		if (vkCreateShaderModule( m_LogicalDevice, &vkInfo, nullptr, &module ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Failed to create shader module! (Vulkan)" );
			return false;
		}

	}

	void VulkanDevice::decode_command_buffer( std::span<uint8> rawBuffer, VkCommandBuffer& vkBuffer ) noexcept {

		uint8* current = rawBuffer.data( );
		const uint8* end = current + rawBuffer.size( );

		while (current < end) {

			CommandType type = *reinterpret_cast<CommandType*>( current );
			current += sizeof( CommandType );
			m_CommandProcessor.ExecuteCall( type, vkBuffer, current );

		}

	}

	void VulkanDevice::transition_image_layout( const detail::TransitionImageInfo& info ) noexcept {
		
		LUM_ASSERT( info.m_Image, "Image cannot be null, fix your code." );
		LUM_ASSERT( info.m_VkCommandBuffer != VK_NULL_HANDLE, "VkCommandBuffer is null." );
		
		auto& imageInfo = info.m_Image( ).m_Info;

		bool sameLayout = imageInfo.m_CurrentLayout == info.m_NewLayout;
		bool sameStage = info.m_Image( ).m_VkStageMask == info.m_NewStage;
		bool sameAccess = info.m_Image( ).m_VkAccessMask == info.m_NewAccess;

		if (sameLayout && sameAccess && sameStage) {
			return;
		}

		VkImageMemoryBarrier2 barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;

		barrier.srcStageMask = info.m_Image().m_VkStageMask;
		barrier.srcAccessMask = info.m_Image().m_VkAccessMask;
		barrier.oldLayout = ToVk( imageInfo.m_CurrentLayout );

		barrier.dstStageMask = info.m_NewStage;
		barrier.dstAccessMask = info.m_NewAccess;
		barrier.newLayout = ToVk( info.m_NewLayout );

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = info.m_Image().m_VkImage;
		barrier.subresourceRange.aspectMask = GetAspectMask( imageInfo.m_Format );
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = imageInfo.m_MipmapLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = imageInfo.m_ArrayLayers;

		VkDependencyInfo dependency{};
		dependency.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependency.imageMemoryBarrierCount = 1;
		dependency.pImageMemoryBarriers = &barrier;

		vkCmdPipelineBarrier2( info.m_VkCommandBuffer, &dependency );

		imageInfo.m_CurrentLayout = info.m_NewLayout;
		info.m_Image( ).m_VkStageMask = info.m_NewStage;
		info.m_Image( ).m_VkAccessMask = info.m_NewAccess;

	}


} // namespace lum::rhi::vk