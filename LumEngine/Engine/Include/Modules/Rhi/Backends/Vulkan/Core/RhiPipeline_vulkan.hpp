#pragma once
#include <vulkan/vulkan_core.h>

namespace lum::rhi::vk {

	struct VulkanPipeline {

		VkPipeline m_VkPipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_VkLayout = VK_NULL_HANDLE;

	};

} // namespace lum::rhi::vk
