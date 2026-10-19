#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/Utils/CommandInterpreter.hpp"

#include "Core/Utils/Assert.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

namespace lum::rhi::vk {

	void CommandInterpreter::RegisterCall( CommandType type, CommandCallFn call ) {

		m_CommandCalls[ type ] = call;

	}
	void CommandInterpreter::ExecuteCall( CommandType type, VkCommandBuffer vkCmdBuf, VulkanDevice& ) {

		LUM_ASSERT(
			m_CommandCalls.contains( type ),
			FormatString( "Unregistered CommandType ({}) encountered during decoding stream. Did you forget to register its handler?", type )
		);


	}

} // namespace  lum::rhi::vk 