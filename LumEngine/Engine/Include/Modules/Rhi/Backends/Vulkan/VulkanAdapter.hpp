#pragma once

#include <vector>

#include <Vulkan/vulkan_core.h>

#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/TypeUtils.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"

namespace lum::rhi::vk {

	class VulkanSurfaceSupport {
	public:

		VkSurfaceCapabilitiesKHR		m_Capabilities{};
		std::vector<VkSurfaceFormatKHR> m_Formats{};
		std::vector<VkPresentModeKHR>	m_PresentModes{};

		LUM_NODISCARD VkSurfaceFormatKHR SelectSurfaceFormat( ) const noexcept;
		LUM_NODISCARD VkPresentModeKHR SelectPresentMode( ) const noexcept;

	};

	class VulkanQueueFamilies {
	public:

		uint32 m_GraphicsQueueIndex = MaxValue<uint32>( );
		uint32 m_PresentQueueIndex = MaxValue<uint32>( ); // IMPLEMENT PRESENT QUEUE INDEX
		uint32 m_ComputeQueueIndex = MaxValue<uint32>( );

		LUM_NODISCARD constexpr bool HasQueue( uint32 index ) const noexcept {
			return index != MaxValue<uint32>( );
		}

		LUM_NODISCARD constexpr bool IsValid( ) const noexcept {
			return HasQueue( m_GraphicsQueueIndex ) && HasQueue( m_PresentQueueIndex );
		}

	};

	class VulkanAdapter {
	public:

		VkPhysicalDevice m_Device = VK_NULL_HANDLE;
		VkPhysicalDeviceFeatures m_Features{};
		VkPhysicalDeviceMemoryProperties m_MemoryProperties{};
		VulkanSurfaceSupport m_SurfaceSupport{};
		VulkanQueueFamilies	m_Queues{};
		AdapterDescription m_Desc{};
		uint32 m_Score = 0;

		void QuerySurfaceCapabilities( VkSurfaceKHR surface ) noexcept;

		uint32 FindMemoryType( uint32 typeBits, VkMemoryPropertyFlags properties ) const noexcept;

	};

} // namespace lum::rhi::vk