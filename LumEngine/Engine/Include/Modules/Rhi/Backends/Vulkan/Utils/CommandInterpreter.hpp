#pragma once

#include "Core/CoreTypes.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

#include <unordered_map>
#include <Vulkan/vulkan.hpp>

namespace lum::rhi::vk {

	class VulkanDevice;

	using CommandCallFn = void (*)(VkCommandBuffer vkCmd, VulkanDevice& device, const uint8* ptr);

	class CommandInterpreter {
	public:

		void RegisterCall( CommandType type, CommandCallFn call );
		void ExecuteCall( CommandType type, VkCommandBuffer vkCmdBuf, VulkanDevice& );

	private:
		
		std::unordered_map<CommandType, CommandCallFn> m_CommandCalls{};
	
	};

} // namespace lum::rhi::Vk