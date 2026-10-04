#pragma once

#include "Rhi2/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	struct VulkanBuffer {

		Buffer2 m_NativeBuffer{};
		VkBuffer m_VkBuffer{};
		VkDeviceMemory m_Memory{};
		VkMemoryRequirements m_MemoryRequirements{};

	};

} // namespace lum::rhi::vk