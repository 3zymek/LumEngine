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

namespace lum::rhi::vk {

	struct VulkanResourceSet {

		VkDescriptorSet m_ResourceSet = VK_NULL_HANDLE;
		VkDescriptorPool m_ResourcePool = VK_NULL_HANDLE;
		VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;

	};

} // namespace lum::rhi::vk