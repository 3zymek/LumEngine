#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

namespace lum::rhi::vk {

	PipelineHandle2 VulkanDevice::CreatePipeline( const PipelineCreateInfo2& info ) noexcept {
		 
		constexpr auto k_MaxShaderStages = ToUnderlyingEnum( ShaderStage::_Count );
		std::array<VkPipelineShaderStageCreateInfo, k_MaxShaderStages> shaderStages{};
		for (usize i = 0; i < info.m_ShaderInfos.size( ); i++) {
			auto& stage = shaderStages[ i ];
			

		}

		return {};

	}

	void VulkanDevice::DestroyPipeline( PipelineHandle2& handle ) noexcept {

	}

} // namespace lum::rhi::vk