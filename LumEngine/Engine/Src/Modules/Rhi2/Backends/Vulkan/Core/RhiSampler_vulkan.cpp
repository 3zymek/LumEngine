#pragma once

#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

namespace lum::rhi::vk {

	SamplerHandle2 VulkanDevice::CreateSampler( const SamplerCreateInfo2& info ) noexcept {

		if (m_Samplers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Sampler: Max samplers reached! Increase max samplers count or refactor your code." );
			return {};
		}

		auto sampler = m_ImageCreator.CreateSampler( info );
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