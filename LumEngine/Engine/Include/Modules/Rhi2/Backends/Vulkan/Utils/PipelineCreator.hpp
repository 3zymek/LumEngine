#pragma once
#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"

namespace lum::rhi::vk {

	class PipelineCreator {
	public:

		void Initialize( const VkDevice logicalDevice ) {
			m_LogicalDevice = logicalDevice;
		}

		VkPipeline CreatePipeline( const PipelineCreateInfo2& info ) {

			std::array<Optional<VkPipelineShaderStageCreateInfo>, s_MaxShaderStages> shaderStages{};
			for (auto& shaderInfo : info.m_ShaderInfos) {

				auto shaderIndex = ToUnderlyingEnum( shaderInfo.m_Stage );
				auto& stage = shaderStages[ shaderIndex ];
				
				if (stage.HasValue( )) {
					LUM_LOG_WARN( "Shader stage slot {} is already occupied, skipping shader '{}'", shaderIndex, shaderInfo.m_Path.ToString( ) );
					continue;
				}
				stage = {};
				stage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stage->pName = shaderInfo.m_EntryPoint.data( );
				stage->stage = s_ShaderStageLookup[ shaderIndex ];
				stage->module = create_shader_module( shaderInfo.m_Path );

				if (shaderInfo.m_Stage < ShaderStage::_Count) {

				}

			}

			static inline constexpr std::array<VkDynamicState, 2> s_DynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};



			VkPipelineDynamicStateCreateInfo dynamicState{};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = SafeCast<uint32>( s_DynamicStates.size( ) );
			dynamicState.pDynamicStates = s_DynamicStates.data( );

			VkPipelineInputAssemblyStateCreateInfo assemblyState{};
			assemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			assemblyState.topology = 

		}

	private:

		VkShaderModule create_shader_module( const Path& path ) {

			auto binaryCode = FileSystem::ReadBinaryFile( path );
			if (!binaryCode) {
				LUM_LOG_ERROR( "Failed to create shader: {}!", binaryCode.GetError( ) );
				return false;
			}

			VkShaderModuleCreateInfo vkInfo{};
			vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vkInfo.pCode = binaryCode->data( );
			vkInfo.codeSize = binaryCode->size( ) / sizeof( uint32 );

			VkShaderModule module = VK_NULL_HANDLE;

			if (vkCreateShaderModule( m_LogicalDevice, &vkInfo, nullptr, &module ) != VK_SUCCESS) {
				LUM_LOG_ERROR( "Failed to create shader module! (Vulkan)" );
				return false;
			}

		}

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		
		static inline constexpr auto s_MaxShaderStages = ToUnderlyingEnum( ShaderStage::_Count );
		static inline constexpr VkShaderStageFlagBits s_ShaderStageLookup[ s_MaxShaderStages ] = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			VK_SHADER_STAGE_COMPUTE_BIT,
			VK_SHADER_STAGE_GEOMETRY_BIT
		};


		static inline constexpr VkPrimitiveTopology s_PrimitiveTopologyLookup[ ToUnderlyingEnum( PrimitiveTopology::_Count ) ] = {
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
			VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
		};

	};

} // namespace lum::rhi::vk