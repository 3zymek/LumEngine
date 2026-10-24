#pragma once

#include <vulkan/vulkan_core.h>

#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	struct VulkanBuffer {

		BufferDescription m_Desc{};
		VkBuffer m_VkBuffer{};
		VkDeviceMemory m_Memory{};
		VkMemoryRequirements m_MemoryRequirements{};

	};

} // namespace lum::rhi::vk