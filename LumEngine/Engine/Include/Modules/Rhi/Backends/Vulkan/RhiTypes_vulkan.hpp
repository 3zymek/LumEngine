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

#include <source_location>

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

				m_13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
				m_13Features.dynamicRendering = VK_TRUE;
				m_13Features.synchronization2 = VK_TRUE;
				m_13Features.pNext = nullptr;

				m_12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
				m_12Features.descriptorIndexing = VK_TRUE;
				m_12Features.pNext = &m_13Features;

			}

			VkPhysicalDeviceVulkan13Features m_13Features{};
			VkPhysicalDeviceVulkan12Features m_12Features{};

		};

		LUM_NODISCARD inline bool ResultValidation( const VkResult res, const char* fnName, const std::source_location loc = std::source_location::current( ) ) noexcept {
			if (res != VK_SUCCESS) {
				LUM_LOG_ERROR(
					"Vulkan error: '{}' failed with VkResult: {}\n  -> At {}:{}",
					fnName,
					static_cast<int32>(res),
					loc.file_name( ),
					loc.line( )
				);
				return false;
			}
			return true;
		}

#		define LUM_VK_RESULT_CHECK(call) \
			lum::rhi::vk::detail::ResultValidation( call, #call )


	}

} // namespace lum::rhi::vk