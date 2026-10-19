#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

#include "Core/CoreTypes.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/ImageCreator.hpp"
#include "Rhi/Backends/Vulkan/Utils/PipelineCreator.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::vk {


	BufferHandle2 VulkanDevice::CreateBuffer( const BufferCreateInfo2& info ) noexcept {

		assert_device( );

		if (m_Buffers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create buffer: Max buffers reached! Increase max buffers count or refactor your code." );
			return {};
		}

		VkBufferUsageFlags usage{};
		if (info.m_Usage.Has( BufferUsage2::Vertex )) {
			usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::Element )) {
			usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::Uniform )) {
			usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::ShaderStorage )) {
			usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}
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

		vkGetBufferMemoryRequirements( m_LogicalDevice, vkBuffer.m_VkBuffer, &vkBuffer.m_MemoryRequirements );

		uint32 memoryTypeIndex = m_Adapter.FindMemoryType(
			vkBuffer.m_MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		allocInfo.allocationSize = vkBuffer.m_MemoryRequirements.size;

		if (vkAllocateMemory( m_LogicalDevice, &allocInfo, nullptr, &vkBuffer.m_Memory ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't allocate buffer memory" );
			vkDestroyBuffer( m_LogicalDevice, vkBuffer.m_VkBuffer, nullptr );
			return {};
		}

		BufferHandle2 bufferHandle{};

		if (vkBindBufferMemory( m_LogicalDevice, vkBuffer.m_VkBuffer, vkBuffer.m_Memory, 0 ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create buffer: Binding buffer memory failed" );
			return {};
		}
		else {
			bufferHandle = m_Buffers.Append( vkBuffer );
		}

		// Data insertion
		if (info.m_Data != nullptr) {

			void* data = nullptr;
			VkResult res = vkMapMemory( m_LogicalDevice, vkBuffer.m_Memory, 0, info.m_DataSize, 0, &data );
			if (res != VK_SUCCESS) {
				LUM_LOG_WARN( "Couldn't map buffer memory!" );
				return bufferHandle;
			}
			memcpy( data, info.m_Data, info.m_DataSize );
			vkUnmapMemory( m_LogicalDevice, vkBuffer.m_Memory );
		}

		return bufferHandle;

	}
	void VulkanDevice::DestroyBuffer( BufferHandle2& handle ) noexcept {

		assert_device( );

		if (!m_Buffers.Contains( handle )) return;

		auto& buffer = m_Buffers[ handle ];

		vkDeviceWaitIdle( m_LogicalDevice );

		vkDestroyBuffer( m_LogicalDevice, buffer.m_VkBuffer, nullptr );
		vkFreeMemory( m_LogicalDevice, buffer.m_Memory, nullptr );

		m_Buffers.Remove( handle );

	}

	PipelineHandle2 VulkanDevice::CreatePipeline( const PipelineCreateInfo2& info ) noexcept {

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
	void VulkanDevice::DestroyPipeline( PipelineHandle2& handle ) noexcept {

		assert_device( );

		if (!m_Pipelines.Contains( handle )) return;

		auto& pipeline = m_Pipelines[ handle ];

		vkDeviceWaitIdle( m_LogicalDevice );

		vkDestroyPipeline( m_LogicalDevice, pipeline.m_Pipeline, nullptr );
		vkDestroyPipelineLayout( m_LogicalDevice, pipeline.m_Layout, nullptr );

		m_Pipelines.Remove( handle );

	}

	ImageHandle2 VulkanDevice::CreateImage( const ImageCreateInfo2& info ) noexcept {

		if (m_Images.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Image: Max images reached! Increase max images count or refactor your code." );
			return {};
		}

		auto image = ImageResourceCreator{}.CreateImage( m_LogicalDevice, m_Adapter, info );
		if (!image) {
			LUM_LOG_ERROR( "Failed to create RHI Image: {}", image.GetError( ) );
			return {};
		}

		return m_Images.Append( image.ValueRef( ) );

	}
	void VulkanDevice::DestroyImage( ImageHandle2& handle ) noexcept {

		if (!m_Images.Contains( handle )) return;

		auto& image = m_Images[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (image.m_Image != VK_NULL_HANDLE)
			vkDestroyImage( m_LogicalDevice, image.m_Image, nullptr );
		if (image.m_View != VK_NULL_HANDLE)
			vkDestroyImageView( m_LogicalDevice, image.m_View, nullptr );

		m_Images.Remove( handle );

	}

	SamplerHandle2 VulkanDevice::CreateSampler( const SamplerCreateInfo2& info ) noexcept {

		if (m_Samplers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Sampler: Max samplers reached! Increase max samplers count or refactor your code." );
			return {};
		}

		auto sampler = ImageResourceCreator{}.CreateSampler( m_LogicalDevice, info );
		if (!sampler) {
			LUM_LOG_ERROR( "Couldn't create RHI Sampler: {}", sampler.GetError( ) );
			return {};
		}

		return m_Samplers.Append( sampler.ValueRef( ) );

	}
	void VulkanDevice::DestroySampler( SamplerHandle2& handle ) noexcept {

		if (!m_Samplers.Contains( handle )) return;

		auto& sampler = m_Samplers[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (sampler.m_Sampler != VK_NULL_HANDLE)
			vkDestroySampler( m_LogicalDevice, sampler.m_Sampler, nullptr );

		m_Samplers.Remove( handle );

	}



} // namespace lum::rhi::vk