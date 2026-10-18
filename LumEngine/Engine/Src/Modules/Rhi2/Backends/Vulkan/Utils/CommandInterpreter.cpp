#include "Rhi2/Backends/Vulkan/Utils/CommandInterpreter.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

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