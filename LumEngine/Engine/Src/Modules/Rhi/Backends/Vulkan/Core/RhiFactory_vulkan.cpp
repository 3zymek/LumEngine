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
#include "Rhi/Backends/Vulkan/Utils/BufferFactory.hpp"
#include "Rhi/Backends/Vulkan/Utils/ImageFactory.hpp"
#include "Rhi/Backends/Vulkan/Utils/PipelineFactory.hpp"
#include "Rhi/Backends/Vulkan/Utils/ResourceSetFactory.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::vk {


	BufferHandle VulkanDevice::CreateBuffer( const BufferCreateInfo& info ) noexcept {

		assert_device( );

		if (m_Buffers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create buffer: Max buffers reached! Increase max buffers count or refactor your code." );
			return {};
		}

		auto buffer = BufferFactory{}.CreateBuffer( m_LogicalDevice, m_Adapter, info );
		if (!buffer) {
			LUM_LOG_WARN( "Couldn't create RHI Buffer: {}", buffer.GetError( ) );
			return {};
		}

		return m_Buffers.Append( buffer.ValueRef( ) );

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

		auto pipeline = PipelineFactory{}.CreatePipeline( *this, info );
		if (!pipeline) {
			LUM_LOG_ERROR( "Couldn't create RHI Pipeline: {}", pipeline.GetError( ) );
			return {};
		}

		return m_Pipelines.Append( pipeline.ValueRef( ) );

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

		assert_device( );

		if (m_Images.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Image: Max Images reached! Increase max Images count or refactor your code." );
			return {};
		}

		auto image = ImageResourceFactory{}.CreateImage( m_LogicalDevice, m_Adapter, info );
		if (!image) {
			LUM_LOG_ERROR( "Couldn't create RHI Image: {}", image.GetError( ) );
			return {};
		}

		return m_Images.Append( image.ValueRef( ) );

	}
	void VulkanDevice::DestroyImage( const ImageHandle handle ) noexcept {

		assert_device( );

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

		assert_device( );

		if (m_Samplers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Sampler: Max Samplers reached! Increase max Samplers count or refactor your code." );
			return {};
		}

		auto sampler = ImageResourceFactory{}.CreateSampler( m_LogicalDevice, info );
		if (!sampler) {
			LUM_LOG_ERROR( "Couldn't create RHI Sampler: {}", sampler.GetError( ) );
			return {};
		}

		return m_Samplers.Append( sampler.ValueRef( ) );

	}
	void VulkanDevice::DestroySampler( const SamplerHandle handle ) noexcept {

		assert_device( );

		if (!m_Samplers.Contains( handle )) return;

		auto& sampler = m_Samplers[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (sampler.m_VkSampler != VK_NULL_HANDLE)
			vkDestroySampler( m_LogicalDevice, sampler.m_VkSampler, nullptr );

		m_Samplers.Remove( handle );

	}

	ResourceLayoutHandle VulkanDevice::CreateResourceLayout( const ResourceLayoutCreateInfo& info ) noexcept {

		assert_device( );

		if (m_ResourceLayouts.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI ResourceLayout: Max ResourceLayouts reached! Increase max ResourceLayouts count or refactor your code." );
			return {};
		}

		auto layout = ResourceSetFactory{}.CreateResourceLayout( m_LogicalDevice, info );
		if (!layout) {
			LUM_LOG_ERROR( "Couldn't create RHI ResourceLayout: {}", layout.GetError( ) );
			return {};
		}

		return m_ResourceLayouts.Append( layout.ValueRef( ) );
	}
	void VulkanDevice::DestroyResourceLayout( const ResourceLayoutHandle handle ) noexcept {

		assert_device( );

		if (!m_ResourceLayouts.Contains( handle )) return;

		auto& layout = m_ResourceLayouts[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (layout.m_VkLayout != VK_NULL_HANDLE) {
			vkDestroyDescriptorSetLayout( m_LogicalDevice, layout.m_VkLayout, nullptr );
		}

		m_ResourceLayouts.Remove( handle );

	}

	ResourceSetHandle VulkanDevice::CreateResourceSet( const ResourceSetCreateInfo& info ) noexcept {

		assert_device( );

		if (m_ResourceSets.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI ResourceSet: Max ResourceSets reached! Increase max ResourceSets count or refactor your code." );
			return {};
		}

		auto resourceSet = ResourceSetFactory{}.CreateResourceSet( *this, info );
		if (!resourceSet) {
			LUM_LOG_ERROR( "Couldn't create RHI ResourceSet: {}", resourceSet.GetError( ) );
			return {};
		}

		return m_ResourceSets.Append( resourceSet.ValueRef( ) );
	}
	void VulkanDevice::DestroyResourceSet( const ResourceSetHandle handle ) noexcept {

		assert_device( );

		if (!m_ResourceSets.Contains( handle )) return;

		auto& set = m_ResourceSets[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (set.m_VkResourcePool != VK_NULL_HANDLE) {
			vkDestroyDescriptorPool( m_LogicalDevice, set.m_VkResourcePool, nullptr );
		}

		m_ResourceSets.Remove( handle );

	}


} // namespace lum::rhi::vk