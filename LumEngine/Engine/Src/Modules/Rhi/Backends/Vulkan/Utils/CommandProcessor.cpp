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
#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/Utils/CommandProcessor.hpp"

#include "Core/Utils/Assert.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

namespace lum::rhi::vk {

	void VulkanCommandProcessor::ExecuteCall( CommandType type, VkCommandBuffer vkCmdBuffer, uint8*& ptr ) noexcept {

		auto typeIndex = ToUnderlyingEnum( type );
		auto& call = m_CommandCalls[ typeIndex ];

		LUM_ASSERT(
			call.m_InvokeFn,
			FormatString( "Unregistered CommandType ({}) encountered during decoding stream. Did you forget to register its handler?", type )
		);

		
		call.m_InvokeFn( &call.m_Storage, vkCmdBuffer, ptr );

	}

} // namespace  lum::rhi::vk 