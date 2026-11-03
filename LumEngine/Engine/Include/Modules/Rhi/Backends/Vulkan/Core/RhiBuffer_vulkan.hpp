#pragma once

#include <Vulkan/vulkan_core.h>

#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	struct VulkanBuffer {

		BufferInfo m_Info{};
		VkBuffer m_VkBuffer{};
		VkDeviceMemory m_VkMemory{};
		VkMemoryRequirements m_VkMemoryRequirements{};

	};

} // namespace lum::rhi::vk