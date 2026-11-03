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

#include <Vulkan/vulkan_core.h>

#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"

namespace lum::rhi::vk {

	struct VulkanResourceSet {

		VkDescriptorSet m_VkResourceSet = VK_NULL_HANDLE;
		VkDescriptorPool m_VkResourcePool = VK_NULL_HANDLE;

		ResourceSetCreateInfo m_Info{};

	};

	struct VulkanResourceLayout {

		VkDescriptorSetLayout m_VkLayout = VK_NULL_HANDLE;

		ResourceLayoutCreateInfo m_Info{};

	};

} // namespace lum::rhi::vk