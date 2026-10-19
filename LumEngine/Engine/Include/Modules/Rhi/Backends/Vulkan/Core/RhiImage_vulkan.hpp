#pragma once
#include <vulkan/vulkan_core.h>

namespace lum::rhi::vk {

	struct VulkanImage {

		VkImage m_Image = VK_NULL_HANDLE;
		VkImageView m_View = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;

	};

} // namespace lum::rhi::vk