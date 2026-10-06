#pragma once

#include "Rhi2/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi::vk {

	struct VulkanShader {

		ShaderInfo2 m_ShaderInfo{};
		VkShaderModule m_VkShader{};

	};

	struct VulkanShaderProgram {

		std::array<Optional<VulkanShader>, static_cast<usize>( ShaderStage::_Count )> m_Shaders{};

	};

} // namespace lum::rhi::vk