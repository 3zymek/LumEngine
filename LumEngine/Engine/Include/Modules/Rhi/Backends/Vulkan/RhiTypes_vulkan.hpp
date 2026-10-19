#pragma once

#include <vulkan/vulkan_core.h>

#include "Core/CoreDefines.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum::rhi::vk {

	namespace detail {

		class LogicalDeviceFeatures {
		public:

			LUM_NODISCARD static const LogicalDeviceFeatures& GetStatic( ) noexcept {
				static LogicalDeviceFeatures val{};
				return val;
			}

			LUM_NODISCARD const void* GetChainHead( ) const noexcept {
				return &m_12Features;
			}

		private:

			LogicalDeviceFeatures( ) { create_defaults( ); }

			void create_defaults( ) {

				//m_Vulkan14Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;
				//m_Vulkan14Features.dynamicRenderingLocalRead = VK_TRUE;
				//m_Vulkan14Features.pNext = nullptr;

				m_13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
				m_13Features.dynamicRendering = VK_TRUE;
				m_13Features.synchronization2 = VK_TRUE;
				m_13Features.pNext = nullptr;

				m_12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
				m_12Features.descriptorIndexing = VK_TRUE;
				m_12Features.pNext = &m_13Features;

			}

			//VkPhysicalDeviceVulkan14Features m_Vulkan14Features{};
			VkPhysicalDeviceVulkan13Features m_13Features{};
			VkPhysicalDeviceVulkan12Features m_12Features{};

		};


	}

} // namespace lum::rhi::vk