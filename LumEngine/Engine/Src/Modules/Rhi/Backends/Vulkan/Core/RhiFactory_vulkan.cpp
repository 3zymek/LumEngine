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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/ImageCreator.hpp"
#include "Rhi/Backends/Vulkan/Utils/PipelineCreator.hpp"
#include "Rhi/Backends/Vulkan/Utils/ResourceSetCreator.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"

namespace lum::rhi::vk {


	BufferHandle VulkanDevice::CreateBuffer( const BufferCreateInfo& info ) noexcept {

		assert_device( );

		if (m_Buffers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create buffer: Max buffers reached! Increase max buffers count or refactor your code." );
			return {};
		}

		auto usage = ToVk( info.m_Usage );
		if (usage == 0) {
			LUM_LOG_WARN( "Couldn't create buffer: 0 usage flags in BufferCreateInfo!" );
			return {};
		}

		VkBufferCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfo.size = info.m_BufferSize;
		vkInfo.usage = usage;

		VulkanBuffer vkBuffer{};

		if (vkCreateBuffer( m_LogicalDevice, &vkInfo, nullptr, &vkBuffer.m_VkBuffer ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create buffer: Creation failed" );
			return {};
		}

		vkGetBufferMemoryRequirements( m_LogicalDevice, vkBuffer.m_VkBuffer, &vkBuffer.m_VkMemoryRequirements );

		uint32 memoryTypeIndex = m_Adapter.FindMemoryType(
			vkBuffer.m_VkMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		allocInfo.allocationSize = vkBuffer.m_VkMemoryRequirements.size;

		if (vkAllocateMemory( m_LogicalDevice, &allocInfo, nullptr, &vkBuffer.m_VkMemory ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't allocate buffer memory" );
			vkDestroyBuffer( m_LogicalDevice, vkBuffer.m_VkBuffer, nullptr );
			return {};
		}

		BufferHandle bufferHandle{};

		if (vkBindBufferMemory( m_LogicalDevice, vkBuffer.m_VkBuffer, vkBuffer.m_VkMemory, 0 ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create buffer: Binding buffer memory failed" );
			return {};
		}
		else {
			bufferHandle = m_Buffers.Append( vkBuffer );
		}

		// Data insertion
		if (info.m_Data != nullptr) {

			void* data = nullptr;
			VkResult res = vkMapMemory( m_LogicalDevice, vkBuffer.m_VkMemory, 0, info.m_DataSize, 0, &data );
			if (res != VK_SUCCESS) {
				LUM_LOG_WARN( "Couldn't map buffer memory!" );
				return bufferHandle;
			}
			memcpy( data, info.m_Data, info.m_DataSize );
			vkUnmapMemory( m_LogicalDevice, vkBuffer.m_VkMemory );
		}

		return bufferHandle;

	}
	void VulkanDevice::DestroyBuffer( const BufferHandle handle ) noexcept {

		assert_device( );

		if (!m_Buffers.Contains( handle )) return;

		auto& buffer = m_Buffers[ handle ];

		vkDeviceWaitIdle( m_LogicalDevice );

		vkDestroyBuffer( m_LogicalDevice, buffer.m_VkBuffer, nullptr );
		vkFreeMemory( m_LogicalDevice, buffer.m_VkMemory, nullptr );

		m_Buffers.Remove( handle );

	}

	PipelineHandle VulkanDevice::CreatePipeline( const PipelineCreateInfo& info ) noexcept {

		assert_device( );

		if (m_Pipelines.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Pipeline: Max pipelines reached! Increase max pipelines count or refactor your code." );
			return {};
		}

		auto pipeline = PipelineCreator{}.CreatePipeline( m_LogicalDevice, m_Adapter, info );
		if (!pipeline) {
			LUM_LOG_ERROR( "Couldn't create pipeline: {}", pipeline.GetError( ) );
			return {};
		}

		auto pipelineHandle = m_Pipelines.Append( pipeline.ValueRef( ) );

		return pipelineHandle;

	}
	void VulkanDevice::DestroyPipeline( const PipelineHandle handle ) noexcept {

		assert_device( );

		if (!m_Pipelines.Contains( handle )) return;

		auto& pipeline = m_Pipelines[ handle ];

		vkDeviceWaitIdle( m_LogicalDevice );

		vkDestroyPipeline( m_LogicalDevice, pipeline.m_VkPipeline, nullptr );
		vkDestroyPipelineLayout( m_LogicalDevice, pipeline.m_VkLayout, nullptr );

		m_Pipelines.Remove( handle );

	}

	ImageHandle VulkanDevice::CreateImage( const ImageCreateInfo& info ) noexcept {

		if (m_Images.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Image: Max Images reached! Increase max Images count or refactor your code." );
			return {};
		}

		auto image = ImageResourceCreator{}.CreateImage( m_LogicalDevice, m_Adapter, info );
		if (!image) {
			LUM_LOG_ERROR( "Failed to create RHI Image: {}", image.GetError( ) );
			return {};
		}

		return m_Images.Append( image.ValueRef( ) );

	}
	void VulkanDevice::DestroyImage( const ImageHandle handle ) noexcept {

		if (!m_Images.Contains( handle )) return;

		auto& image = m_Images[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (image.m_VkImage != VK_NULL_HANDLE)
			vkDestroyImage( m_LogicalDevice, image.m_VkImage, nullptr );
		if (image.m_VkView != VK_NULL_HANDLE)
			vkDestroyImageView( m_LogicalDevice, image.m_VkView, nullptr );

		m_Images.Remove( handle );

	}

	SamplerHandle VulkanDevice::CreateSampler( const SamplerCreateInfo& info ) noexcept {

		if (m_Samplers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Sampler: Max Samplers reached! Increase max Samplers count or refactor your code." );
			return {};
		}

		auto sampler = ImageResourceCreator{}.CreateSampler( m_LogicalDevice, info );
		if (!sampler) {
			LUM_LOG_ERROR( "Couldn't create RHI Sampler: {}", sampler.GetError( ) );
			return {};
		}

		return m_Samplers.Append( sampler.ValueRef( ) );

	}
	void VulkanDevice::DestroySampler( const SamplerHandle handle ) noexcept {

		if (!m_Samplers.Contains( handle )) return;

		auto& sampler = m_Samplers[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (sampler.m_VkSampler != VK_NULL_HANDLE)
			vkDestroySampler( m_LogicalDevice, sampler.m_VkSampler, nullptr );

		m_Samplers.Remove( handle );

	}

	ResourceSetHandle VulkanDevice::CreateResourceSet( const ResourceSetCreateInfo& info ) noexcept {

		if (m_ResourceSets.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI ResourceSet: Max ResourceSets reached! Increase max ResourceSets count or refactor your code." );
			return {};
		}

		auto resourceSet = ResourceSetCreator{}.CreateResourceSet( m_LogicalDevice, info );
		if (!resourceSet) {
			LUM_LOG_ERROR( "Couldn't create RHI ResourceSet: {}", resourceSet.GetError( ) );
			return {};
		}

		return m_ResourceSets.Append( resourceSet.ValueRef() );
	}
	void VulkanDevice::DestroyResourceSet( const ResourceSetHandle handle ) noexcept {

		if (!m_ResourceSets.Contains( handle )) return;

		auto& set = m_ResourceSets[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (set.m_VkResourcePool != VK_NULL_HANDLE) {
			vkDestroyDescriptorPool( m_LogicalDevice, set.m_VkResourcePool, nullptr );
		}
		if (set.m_VkLayout != VK_NULL_HANDLE) {
			vkDestroyDescriptorSetLayout( m_LogicalDevice, set.m_VkLayout, nullptr );
		}

		m_ResourceSets.Remove( handle );

	}


} // namespace lum::rhi::vk