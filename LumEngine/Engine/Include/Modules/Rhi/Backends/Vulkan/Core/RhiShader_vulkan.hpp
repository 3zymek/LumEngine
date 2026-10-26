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
#pragma once

#include <vulkan/vulkan_core.h>

#include "Core/Utils/Optional.hpp"
#include "Core/Utils/TypeUtils.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi::vk {

	struct VulkanShader {

		ShaderInfo m_Info{};
		VkShaderModule m_VkShader{};

	};

	struct VulkanShaderProgram {

		std::array<Optional<VulkanShader>, t_EnumCount<ShaderStage>> m_Shaders{};

	};

} // namespace lum::rhi::vk