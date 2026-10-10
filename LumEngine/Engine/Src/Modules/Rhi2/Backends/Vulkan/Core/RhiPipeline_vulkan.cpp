#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

namespace lum::rhi::vk {

	PipelineHandle2 VulkanDevice::CreatePipeline( const PipelineCreateInfo2& info ) noexcept {
		
		assert_device( );

		if (m_Pipelines.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create pipeline: Max pipelines reached! Increase max pipelines count or refactor your code." );
			return {};
		}

		auto pipeline = m_PipelineCreator.CreatePipeline( info );
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

} // namespace lum::rhi::vk