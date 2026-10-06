#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Platform/FileSystem/FileSystem.hpp"

namespace lum::rhi::vk {

	bool VulkanDevice::create_shader_module( const Path& path, VkShaderModule& module ) const noexcept {

		auto binaryCode = FileSystem::ReadBinaryFile( path );
		if (!binaryCode) {
			LUM_LOG_ERROR( "Failed to create shader: {}!", binaryCode.GetError( ) );
			return false;
		}

		VkShaderModuleCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkInfo.pCode = binaryCode->data( );
		vkInfo.codeSize = binaryCode->size( ) / sizeof( uint32 );

		if (vkCreateShaderModule( m_LogicalDevice, &vkInfo, nullptr, &module ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Failed to create shader module! (Vulkan)" );
			return false;
		}

	}

} // namespace lum::rhi::vk