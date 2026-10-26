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

#include "Rhi/Interfaces/Core/RhiImage.hpp"

namespace lum::rhi::vk {

	struct VulkanImage {

		ImageInfo m_Info{};

		VkPipelineStageFlags2 m_VkStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		VkAccessFlags2 m_VkAccessMask = VK_ACCESS_2_NONE;

		VkImage m_VkImage = VK_NULL_HANDLE;
		VkImageView m_VkView = VK_NULL_HANDLE;
		VkDeviceMemory m_VkMemory = VK_NULL_HANDLE;

	};

} // namespace lum::rhi::vk