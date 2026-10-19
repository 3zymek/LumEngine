#pragma once
#include <vulkan/vulkan_core.h>

namespace lum::rhi::vk {

	struct VulkanPipeline {

		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_Layout = VK_NULL_HANDLE;

	};

} // namespace lum::rhi::vk
