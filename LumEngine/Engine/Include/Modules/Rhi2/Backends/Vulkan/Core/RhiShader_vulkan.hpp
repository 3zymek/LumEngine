#pragma once

#include "Rhi2/Interfaces/Core/RhiShader.hpp"
#include "MagicEnum/magic_enum.hpp"

namespace lum::rhi::vk {

	struct VulkanShader {

		ShaderInfo2 m_ShaderInfo{};
		VkShaderModule m_VkShader{};

	};

	struct VulkanShaderProgram {

		std::array<Optional<VulkanShader>, t_EnumCount<ShaderStage>> m_Shaders{};

	};

} // namespace lum::rhi::vk