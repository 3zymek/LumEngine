#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/Utils/CommandInterpreter.hpp"

#include "Core/Utils/Assert.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

namespace lum::rhi::vk {

	void VulkanCommandInterpreter::ExecuteCall( CommandType type, VkCommandBuffer vkCmdBuffer, uint8*& ptr ) noexcept {

		auto typeIndex = ToUnderlyingEnum( type );
		auto& call = m_CommandCalls[ typeIndex ];

		LUM_ASSERT(
			call.m_InvokeFn,
			FormatString( "Unregistered CommandType ({}) encountered during decoding stream. Did you forget to register its handler?", type )
		);

		
		call.m_InvokeFn( &call.m_Storage, vkCmdBuffer, ptr );

	}

} // namespace  lum::rhi::vk 