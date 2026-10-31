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

#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	class VulkanAdapter;

	class BufferFactory {
	public:

		Result<VulkanBuffer> CreateBuffer( VkDevice logicalDevice, VulkanAdapter& adapter, const BufferCreateInfo& info ) const noexcept;

	};

} // namespace lum::rhi::vk