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

// Header implementation file
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

#include "Core/Utils/Assert.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Platform/FileSystem/FileSystem.hpp"
#include "Platform/FileSystem/Path.hpp"
#include "Platform/VulkanSurfaceProvider.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiImage_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
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


		create_vk_instance( info );
		volkLoadInstance( m_Instance );
		create_main_surface( );
		choose_adapter( );
		create_logical_device( );
		acquire_queues( );
		recreate_swapchain( );
		recreate_swapchain_images( );

		// ===========================================================
		// DEBUG
		// ===========================================================

		auto data = ResourceLoader::LoadImageFromFile( ResourceRoot::External, "test.png" );

		LUM_ASSERT( data, "test.png doesn't exists" );

		rhi::ImageExtent3D extent{};
		extent.m_Width = data.ValueRef( ).m_Width;
		extent.m_Height = data.ValueRef( ).m_Height;
		extent.m_Depth = 1;

		rhi::ImageCreateInfo img{};
		img.m_Data = data.ValueRef( );
		img.m_Dimension = rhi::ImageDimension::Image2D;
		img.m_Format = rhi::ImageFormat::RGBA32_FLOAT;
		img.m_ArrayLayers = 1;
		img.m_Extent = extent;
		img.m_MipmapLevels = 1;
		img.m_UsageFlags = rhi::ImageUsageFlag::ColorAttachment;
		img.m_SampleCount = rhi::SampleCount::Sample1;

		DT_Image = CreateImage( img );

		rhi::SamplerCreateInfo smp{};
		DT_Sampler = CreateSampler( smp );


		VulkanImage& image = m_Images[ DT_Image ];
		VulkanSampler& sampler = m_Samplers[ DT_Sampler ];



		VkDescriptorSetLayoutBinding binding{};
		binding.binding = 0;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.descriptorCount = 1;
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &binding;

		if (!LUM_VK_RESULT_CHECK( vkCreateDescriptorSetLayout( m_LogicalDevice, &layoutInfo, nullptr, &DT_DescriptorSetLayout ) )) {
			return;
		}

		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = 1;

		if (!LUM_VK_RESULT_CHECK( vkCreateDescriptorPool( m_LogicalDevice, &poolInfo, nullptr, &DT_DescriptorPool ) )) {
			return;
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = DT_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &DT_DescriptorSetLayout;

		if (!LUM_VK_RESULT_CHECK( vkAllocateDescriptorSets( m_LogicalDevice, &allocInfo, &DT_DescriptorSet ) )) {
			return;
		}

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = image.m_View;
		imageInfo.sampler = sampler.m_Sampler;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = DT_DescriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets( m_LogicalDevice, 1, &descriptorWrite, 0, nullptr );

		// ===========================================================
		// DEBUG
		// ===========================================================

		create_main_pipeline( );
		create_command_pool( );
		allocate_command_buffers( );
		create_sync_primitives( );

		info.m_EventBus( ).SubscribePermanently<EWindowResized>(
			[ & ]( const EWindowResized& e ) {
				m_WindowSize = { e.m_Width, e.m_Height };
			}
		);

		create_vertex_buffers( );

	}

	void VulkanDevice::Finalize( ) noexcept {

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

			vkDestroyFence( m_LogicalDevice, m_Fence, nullptr );

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

		process_commands( );

		// Sync GPU and CPU
		vkWaitForFences( m_LogicalDevice, 1, &m_Fence, VK_TRUE, UINT64_MAX );



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
		vkResetFences( m_LogicalDevice, 1, &m_Fence );





		// Begin recording
		VkCommandBuffer& commandBuffer = m_CmdBuffers[ 0 ];
		vkResetCommandBuffer( commandBuffer, 0 );

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer( commandBuffer, &beginInfo );





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

		VkExtent2D currentExtent = m_Adapter.m_SurfaceSupport.m_Capabilities.currentExtent;

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.renderArea = { { 0, 0 }, currentExtent };
		renderingInfo.layerCount = 1;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments = &colorAttachment;

		vkCmdBeginRendering( commandBuffer, &renderingInfo );

		vkCmdBindPipeline( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_MainPipeline.m_Pipeline );

		VkViewport viewport{ 0.0f, 0.0f, (float32) currentExtent.width, (float32) currentExtent.height, 0.0f, 1.0f };
		VkRect2D scissor{ { 0, 0 }, currentExtent };
		vkCmdSetViewport( commandBuffer, 0, 1, &viewport );
		vkCmdSetScissor( commandBuffer, 0, 1, &scissor );


		VkDeviceSize offset = 0;

		vkCmdBindVertexBuffers(
			commandBuffer,
			0,
			1,
			&m_Buffers[ DT_VertexBuffer ].m_VkBuffer,
			&offset
		);


		vkCmdBindIndexBuffer(
			commandBuffer,
			m_Buffers[ DT_IndexBuffer ].m_VkBuffer,
			0,
			VK_INDEX_TYPE_UINT32
		);


		vkCmdDrawIndexed( commandBuffer, 6, 1, 0, 0, 0 ); // Draw simple triangle

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

		if (!LUM_VK_RESULT_CHECK( vkQueueSubmit2( m_GraphicsQueue, 1, &submitInfo, m_Fence ) )) {
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


	}

	void VulkanDevice::SendCommandBuffer( CommandBuffer&& buffer ) noexcept {

		assert_device( );

		m_CommandBuffersToFlush.emplace( std::move( buffer ) );

	}

	// ===========================================================
	// RUNTIME HELPERS (PRIVATE)
	// ===========================================================

	void VulkanDevice::assert_device( ) const {
		LUM_ASSERT( m_IsInitialized, "RHI Device is not initialized!!!" );
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

		LUM_LOG_INFO( "Number of command buffers to flush: {}", m_CommandBuffersToFlush.size( ) );

		uint8* current = rawBuffer.data( );
		const uint8* end = current + rawBuffer.size( );

		VkCommandBuffer commandBuffer = m_CmdBuffers[ m_CurrentFrame ];

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (!LUM_VK_RESULT_CHECK( vkBeginCommandBuffer( commandBuffer, &beginInfo ) )) {
			return;
		}

		while (current < end) {

			CommandType type = *reinterpret_cast<CommandType*>( current );
			current += sizeof( CommandType );

			m_CommandInterpreter.ExecuteCall( type, commandBuffer, current );

		}

	}
	void VulkanDevice::process_commands( ) noexcept {

		if (m_CommandBuffersToFlush.size( ) == 1) {
			decode_command_buffer( m_CommandBuffersToFlush.front( ).GetByteStream( ), m_CmdBuffers[ m_CurrentFrame ] );
			m_CommandBuffersToFlush.pop( );
			return;

		}

		auto tempQueue = m_CommandBuffersToFlush;
		usize totalSize = 0;
		while (!tempQueue.empty( )) {

			totalSize += tempQueue.front( ).GetByteStream( ).size( );
			tempQueue.pop( );

		}

		std::vector<uint8> combinedBuffer{};
		combinedBuffer.reserve( totalSize );

		while (!m_CommandBuffersToFlush.empty( )) {

			auto buffer = m_CommandBuffersToFlush.front( ).GetByteStream( );

			usize end = combinedBuffer.size( );
			combinedBuffer.resize( end + buffer.size( ) );

			std::memcpy( combinedBuffer.data( ) + end, buffer.data( ), buffer.size_bytes( ) );

			m_CommandBuffersToFlush.pop( );

		}

		decode_command_buffer( combinedBuffer, m_CmdBuffers[ m_CurrentFrame ] );

	}


} // namespace lum::rhi::vk