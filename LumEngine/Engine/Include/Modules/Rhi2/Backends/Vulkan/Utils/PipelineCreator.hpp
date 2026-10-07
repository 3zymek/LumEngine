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

			std::array<Optional<VkPipelineShaderStageCreateInfo>, t_EnumCount<ShaderStage>> shaderStages{};
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
				stage->stage = to_vk( shaderInfo.m_Stage );
				stage->module = create_shader_module( shaderInfo.m_Path );



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
			assemblyState.topology = to_vk( info.m_AssemblyPass.m_Topology );
			assemblyState.primitiveRestartEnable = info.m_AssemblyPass.m_PrimitiveRestart;

			VkPipelineRasterizationStateCreateInfo rasterizationState{};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.cullMode = to_vk( info.m_RasterizationPass.m_CullMode );
			rasterizationState.frontFace = to_vk( info.m_RasterizationPass.m_FrontFace );
			rasterizationState.polygonMode = to_vk( info.m_RasterizationPass.m_PolygonMode );
			rasterizationState.lineWidth = info.m_RasterizationPass.m_WireframeWidth;
			rasterizationState.depthClampEnable = VK_FALSE;
			if (info.m_RasterizationPass.m_DepthBiasEnabled) {
				rasterizationState.depthClampEnable = VK_TRUE;
				rasterizationState.depthBiasConstantFactor = info.m_RasterizationPass.m_DepthBiasConstant;
				rasterizationState.depthBiasSlopeFactor = info.m_RasterizationPass.m_DepthBiasSlope;
				rasterizationState.depthBiasClamp = info.m_RasterizationPass.m_DepthBiasClamp;
			}




		}

	private:

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

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

		[[nodiscard]] static constexpr VkShaderStageFlagBits to_vk( ShaderStage stage ) noexcept {
			switch (stage) {
				case ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
				case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
				case ShaderStage::Compute:  return VK_SHADER_STAGE_COMPUTE_BIT;
				case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
			}
			LUM_ASSERT( false, "Invalid ShaderStage" );
			return {};
		}

		[[nodiscard]] static constexpr VkPrimitiveTopology to_vk( PrimitiveTopology topology ) noexcept {
			switch (topology) {
				case PrimitiveTopology::TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				case PrimitiveTopology::PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
				case PrimitiveTopology::LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
				case PrimitiveTopology::LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			}
			LUM_ASSERT( false, "Invalid PrimitiveTopology enum" );
			return {};
		}

		[[nodiscard]] static constexpr VkCullModeFlags to_vk( CullMode mode ) noexcept {
			switch (mode) {
				case CullMode::None:  return VK_CULL_MODE_NONE;
				case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
				case CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
			}
			LUM_ASSERT( false, "Invalid CullMode enum" );
			return {};
		}

		[[nodiscard]] static constexpr VkPolygonMode to_vk( PolygonMode mode ) noexcept {
			switch (mode) {
				case PolygonMode::Fill: return VK_POLYGON_MODE_FILL;
				case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
			}
			LUM_ASSERT( false, "Invalid PolygonMode enum" );
			return {};
		}

		[[nodiscard]] static constexpr VkFrontFace to_vk( FrontFace face ) noexcept {
			switch (face) {
				case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
				case FrontFace::Clockwise:        return VK_FRONT_FACE_CLOCKWISE;
			}
			LUM_ASSERT( false, "Invalid FrontFace enum" );
			return {};
		}

	};

} // namespace lum::rhi::vk