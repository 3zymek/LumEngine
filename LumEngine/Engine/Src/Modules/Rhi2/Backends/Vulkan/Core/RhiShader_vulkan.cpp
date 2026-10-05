#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Platform/FileSystem/FileSystem.hpp"

namespace lum::rhi::vk {

	ShaderProgramHandle VulkanDevice::CreateShaderProgram( const ShaderProgramCreateInfo& info ) noexcept {

		if (m_ShaderPrograms.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create shader program: Max shader programs reached!" );
			return {};
		}

		if (info.m_Shaders.empty( )) {
			LUM_LOG_WARN( "Couldn't create shader program: Shaders are empty!" );
			return {};
		}

		VulkanShaderProgram program{};

		for (auto& shaderInfo : info.m_Shaders) {

			auto shaderIndex = ToUnderlyingEnum( shaderInfo.m_Stage );
			if (program.m_Shaders[ shaderIndex ].HasValue( )) {
				LUM_LOG_WARN(
					"Shader program already contains a shader for stage {}. Skipping duplicate shader '{}'.",
					ToUnderlyingEnum( shaderInfo.m_Stage ),
					shaderInfo.m_Path
				);
				continue;
			}

			VulkanShader shader{};
			shader.m_ShaderInfo = shaderInfo;
			create_shader( shaderInfo.m_Path, shader.m_VkShader );
			program.m_Shaders[ shaderIndex ] = shader;

		}

		return m_ShaderPrograms.Append( program );

	}

	void VulkanDevice::DestroyShaderProgram( ShaderProgramHandle& handle ) noexcept {


	}

	bool VulkanDevice::create_shader( const Path& path, VkShaderModule& module ) const noexcept {

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