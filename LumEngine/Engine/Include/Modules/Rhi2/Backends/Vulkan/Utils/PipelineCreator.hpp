#pragma once
#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"

namespace lum::rhi::vk {

	class PipelineCreator {
	public:

		void Initialize(const VkDevice logicalDevice) {
			m_LogicalDevice = logicalDevice;
		}

		VkPipeline CreatePipeline(const PipelineCreateInfo2& info) {

			std::array<Optional<VkPipelineShaderStageCreateInfo>, t_EnumCount<ShaderStage>> shaderStages{};
			for (auto& shaderInfo : info.m_ShaderInfos) {

				auto shaderIndex = ToUnderlyingEnum(shaderInfo.m_Stage);
				auto& stage = shaderStages[shaderIndex];

				if (stage.HasValue()) {
					LUM_LOG_WARN("Shader stage slot {} is already occupied, skipping shader '{}'", shaderIndex, shaderInfo.m_Path.ToString());
					continue;
				}
				stage = {};
				stage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stage->pName = shaderInfo.m_EntryPoint.data();
				stage->stage = to_vk(shaderInfo.m_Stage);
				stage->module = create_shader_module(shaderInfo.m_Path);

			}

			static inline constexpr std::array<VkDynamicState, 2> s_DynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			VkPipelineDynamicStateCreateInfo dynamicState{};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = SafeCast<uint32>(s_DynamicStates.size());
			dynamicState.pDynamicStates = s_DynamicStates.data();

			VkPipelineInputAssemblyStateCreateInfo assemblyState{};
			assemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			assemblyState.topology = to_vk(info.m_AssemblyPass.m_Topology);
			assemblyState.primitiveRestartEnable = info.m_AssemblyPass.m_PrimitiveRestart;

			VkPipelineRasterizationStateCreateInfo rasterizationState{};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.cullMode = to_vk(info.m_RasterizationPass.m_CullMode);
			rasterizationState.frontFace = to_vk(info.m_RasterizationPass.m_FrontFace);
			rasterizationState.polygonMode = to_vk(info.m_RasterizationPass.m_PolygonMode);
			rasterizationState.lineWidth = info.m_RasterizationPass.m_WireframeWidth;
			rasterizationState.depthClampEnable = VK_FALSE;
			if (info.m_RasterizationPass.m_DepthBiasEnabled) {
				rasterizationState.depthClampEnable = VK_TRUE;
				rasterizationState.depthBiasConstantFactor = info.m_RasterizationPass.m_DepthBiasConstant;
				rasterizationState.depthBiasSlopeFactor = info.m_RasterizationPass.m_DepthBiasSlope;
				rasterizationState.depthBiasClamp = info.m_RasterizationPass.m_DepthBiasClamp;
			}

			const uint32 numColorAttachments = SafeCast<uint32>(info.m_ColorBlendPass.m_Attachments.size());
			std::vector<VkPipelineColorBlendAttachmentState> colorAttachments(numColorAttachments);
			for (uint32 i = 0; i < numColorAttachments; i++) {

				auto& attachment = colorAttachments[i];
				const auto& attachmentInfo = info.m_ColorBlendPass.m_Attachments[i];
				attachment.blendEnable = attachmentInfo.m_BlendEnabled ? VK_TRUE : VK_FALSE;
				attachment.srcColorBlendFactor = to_vk(attachmentInfo.m_SrcColorBlendFactor);
				attachment.dstColorBlendFactor = to_vk(attachmentInfo.m_DstColorBlendFactor);
				attachment.srcAlphaBlendFactor = to_vk(attachmentInfo.m_SrcAlphaBlendFactor);
				attachment.dstAlphaBlendFactor = to_vk(attachmentInfo.m_DstAlphaBlendFactor);
				attachment.colorBlendOp = to_vk(attachmentInfo.m_ColorBlendOp);
				attachment.alphaBlendOp = to_vk(attachmentInfo.m_AlphaBlendOp);
				attachment.colorWriteMask = to_vk(attachmentInfo.m_ColorMask);

			}

			VkPipelineColorBlendStateCreateInfo colorBlendState{};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.attachmentCount = numColorAttachments;
			colorBlendState.pAttachments = numColorAttachments <= 0 ? nullptr : colorAttachments.data();
			colorBlendState.blendConstants[0] = info.m_ColorBlendPass.m_BlendConstants.m_R;
			colorBlendState.blendConstants[1] = info.m_ColorBlendPass.m_BlendConstants.m_G;
			colorBlendState.blendConstants[2] = info.m_ColorBlendPass.m_BlendConstants.m_B;
			colorBlendState.blendConstants[3] = info.m_ColorBlendPass.m_BlendConstants.m_A;
			colorBlendState.logicOpEnable = VK_FALSE;

			VkPipelineViewportStateCreateInfo viewportState{};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = nullptr;
			viewportState.scissorCount = 1;
			viewportState.pScissors = nullptr;

			VkGraphicsPipelineCreateInfo pipInfo{};
			pipInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipInfo.pDynamicState = &dynamicState;
			pipInfo.pInputAssemblyState = &assemblyState;
			pipInfo.pRasterizationState = &rasterizationState;
			pipInfo.pColorBlendState = &colorBlendState;
			pipInfo.pViewportState = &viewportState;
			//pipInfo.pVertexInputState

		}

	private:

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

		VkShaderModule create_shader_module(const Path& path) {

			auto binaryCode = FileSystem::ReadBinaryFile(path);
			if (!binaryCode) {
				LUM_LOG_ERROR("Failed to create shader: {}!", binaryCode.GetError());
				return false;
			}

			VkShaderModuleCreateInfo vkInfo{};
			vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vkInfo.pCode = binaryCode->data();
			vkInfo.codeSize = binaryCode->size() / sizeof(uint32);

			VkShaderModule module = VK_NULL_HANDLE;

			if (vkCreateShaderModule(m_LogicalDevice, &vkInfo, nullptr, &module) != VK_SUCCESS) {
				LUM_LOG_ERROR("Failed to create shader module! (Vulkan)");
				return false;
			}

		}

		[[nodiscard]] static constexpr VkShaderStageFlagBits to_vk(ShaderStage stage) noexcept {
			switch (stage) {
			case ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::Compute:  return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
			}
			LUM_ASSERT(false, "Invalid ShaderStage");
			return {};
		}

		[[nodiscard]] static constexpr VkPrimitiveTopology to_vk(PrimitiveTopology topology) noexcept {
			switch (topology) {
			case PrimitiveTopology::TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PrimitiveTopology::PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case PrimitiveTopology::LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveTopology::LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			}
			LUM_ASSERT(false, "Invalid PrimitiveTopology enum");
			return {};
		}

		[[nodiscard]] static constexpr VkCullModeFlags to_vk(CullMode mode) noexcept {
			switch (mode) {
			case CullMode::None:  return VK_CULL_MODE_NONE;
			case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
			}
			LUM_ASSERT(false, "Invalid CullMode enum");
			return {};
		}

		[[nodiscard]] static constexpr VkPolygonMode to_vk(PolygonMode mode) noexcept {
			switch (mode) {
			case PolygonMode::Fill: return VK_POLYGON_MODE_FILL;
			case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
			}
			LUM_ASSERT(false, "Invalid PolygonMode enum");
			return {};
		}

		[[nodiscard]] static constexpr VkFrontFace to_vk(FrontFace face) noexcept {
			switch (face) {
			case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case FrontFace::Clockwise:        return VK_FRONT_FACE_CLOCKWISE;
			}
			LUM_ASSERT(false, "Invalid FrontFace enum");
			return {};
		}

		[[nodiscard]] static constexpr VkBlendFactor to_vk(BlendFactor factor) noexcept {
			switch (factor) {
			case BlendFactor::Zero:                  return VK_BLEND_FACTOR_ZERO;
			case BlendFactor::One:                   return VK_BLEND_FACTOR_ONE;
			case BlendFactor::SrcColor:              return VK_BLEND_FACTOR_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DstColor:              return VK_BLEND_FACTOR_DST_COLOR;
			case BlendFactor::OneMinusDstColor:      return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case BlendFactor::SrcAlpha:              return VK_BLEND_FACTOR_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha:              return VK_BLEND_FACTOR_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha:      return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::ConstantColor:         return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha:         return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			}
			LUM_ASSERT(false, "Invalid BlendFactor enum");
			return {};
		}

		[[nodiscard]] static constexpr VkBlendOp to_vk(BlendOp op) noexcept {
			switch (op) {
			case BlendOp::Add:              return VK_BLEND_OP_ADD;
			case BlendOp::Substract:        return VK_BLEND_OP_SUBTRACT;
			case BlendOp::ReverseSubstract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOp::Min:              return VK_BLEND_OP_MIN;
			case BlendOp::Max:              return VK_BLEND_OP_MAX;
			}
			LUM_ASSERT(false, "Invalid BlendOp enum");
			return {};
		}

		[[nodiscard]] static constexpr VkColorComponentFlags to_vk(Flags<ColorComponentFlag> flags) noexcept {

			VkColorComponentFlags vkFlags = 0;

			if (flags.Has(ColorComponentFlag::None)) return 0;
			if (flags.Has(ColorComponentFlag::Red)) vkFlags |= VK_COLOR_COMPONENT_R_BIT;
			if (flags.Has(ColorComponentFlag::Green)) vkFlags |= VK_COLOR_COMPONENT_G_BIT;
			if (flags.Has(ColorComponentFlag::Blue)) vkFlags |= VK_COLOR_COMPONENT_B_BIT;
			if (flags.Has(ColorComponentFlag::Alpha)) vkFlags |= VK_COLOR_COMPONENT_A_BIT;

			return vkFlags;
		}

	};

} // namespace lum::rhi::vk