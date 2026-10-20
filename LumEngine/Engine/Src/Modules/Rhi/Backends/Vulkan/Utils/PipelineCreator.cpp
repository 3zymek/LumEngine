#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/Utils/PipelineCreator.hpp"

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Assert.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Core/Utils/Result.hpp"
#include "Core/Utils/SafeCast.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Core/Utils/TypeUtils.hpp"
#include "Platform/FileSystem/FileSystem.hpp"
#include "Platform/FileSystem/Path.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi::vk {

	//=======================================================//
	// Public
	//=======================================================//

	Result<VulkanPipeline> PipelineCreator::CreatePipeline(const VkDevice logicalDevice, const VulkanAdapter& adapter, const PipelineCreateInfo& info) const noexcept {

		VulkanPipeline pipeline{};

		static constexpr std::array<VkDynamicState, 2> s_DynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = SafeCast<uint32>(s_DynamicStates.size());
		dynamicState.pDynamicStates = s_DynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;

		VkGraphicsPipelineCreateInfo pipInfo{};
		pipInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipInfo.pDynamicState = &dynamicState;

		VkSurfaceFormatKHR surfaceFormat = adapter.m_SurfaceSupport.SelectSurfaceFormat();
		VkPipelineRenderingCreateInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachmentFormats = &surfaceFormat.format;

		// TODO: ADD PIPELINE LAYOUT TO PipelineCreateInfo2
		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 0;
		layoutInfo.pSetLayouts = nullptr;
		layoutInfo.pushConstantRangeCount = 0;
		layoutInfo.pPushConstantRanges = nullptr;

		const VkResult layoutResult =
			vkCreatePipelineLayout(
				logicalDevice,
				&layoutInfo,
				nullptr,
				&pipeline.m_Layout
			);

		if (layoutResult != VK_SUCCESS) {
			return Result<VulkanPipeline>::Failure(
				FormatString("vkCreatePipelineLayout failure (VkResult: {})", static_cast<int32>(layoutResult))
			);
		}

		auto assemblyState = setup_assembly_info(info.m_AssemblyPass);
		pipInfo.pInputAssemblyState = &assemblyState;

		auto rasterizationState = setup_rasterizer_info(info.m_RasterizationPass);
		pipInfo.pRasterizationState = &rasterizationState;

		auto colorBlendState = setup_color_blend_info(info.m_ColorBlendPass);
		pipInfo.pColorBlendState = &colorBlendState;

		auto msState = setup_multisample_info(info.m_MultisamplePass);
		pipInfo.pMultisampleState = &msState;

		auto vertexInputState = setup_vertex_input_info(info.m_VertexInputPass);
		pipInfo.pVertexInputState = &vertexInputState;

		auto shaderStages = setup_shader_modules(logicalDevice, info);
		pipInfo.stageCount = shaderStages.size();
		pipInfo.pStages = shaderStages.data();

		pipInfo.pNext = &renderingInfo;
		pipInfo.pViewportState = &viewportState;
		pipInfo.layout = pipeline.m_Layout;

		const VkResult pipResult =
			vkCreateGraphicsPipelines(
				logicalDevice,
				VK_NULL_HANDLE,
				1,
				&pipInfo,
				nullptr,
				&pipeline.m_Pipeline
			);

		if (pipResult != VK_SUCCESS) {
			return Result<VulkanPipeline>::Failure(
				FormatString("vkCreateGraphicsPipeline failure (VkResult: {})", static_cast<int32>(pipResult))
			);
		}

		for (auto& shader : shaderStages) {
			vkDestroyShaderModule(logicalDevice, shader.module, nullptr);
		}

		return pipeline;

	}




	//=======================================================//
	// Private
	//=======================================================//

	auto PipelineCreator::setup_rasterizer_info(const RasterizationPass& info) const noexcept -> VkPipelineRasterizationStateCreateInfo {

		VkPipelineRasterizationStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		vkInfo.rasterizerDiscardEnable = VK_FALSE;
		vkInfo.cullMode = ToVk(info.m_CullMode);
		vkInfo.frontFace = ToVk(info.m_FrontFace);
		vkInfo.polygonMode = ToVk(info.m_PolygonMode);
		vkInfo.lineWidth = info.m_WireframeWidth;
		vkInfo.depthClampEnable = VK_FALSE;

		if (info.m_DepthBiasEnabled) {
			vkInfo.depthClampEnable = VK_TRUE;
			vkInfo.depthBiasConstantFactor = info.m_DepthBiasConstant;
			vkInfo.depthBiasSlopeFactor = info.m_DepthBiasSlope;
			vkInfo.depthBiasClamp = info.m_DepthBiasClamp;
		}

		return vkInfo;

	}
	auto PipelineCreator::setup_color_blend_info(const ColorBlendPass& info) const noexcept -> VkPipelineColorBlendStateCreateInfo {

		const uint32 numColorAttachments = SafeCast<uint32>(info.m_Attachments.size());
		static std::vector<VkPipelineColorBlendAttachmentState> colorAttachments{};
		colorAttachments.clear();
		colorAttachments.resize(numColorAttachments);

		for (uint32 i = 0; i < numColorAttachments; i++) {

			auto& attachment = colorAttachments[i];
			const auto& attachmentInfo = info.m_Attachments[i];
			attachment.blendEnable = attachmentInfo.m_BlendEnabled ? VK_TRUE : VK_FALSE;
			attachment.srcColorBlendFactor = ToVk(attachmentInfo.m_SrcColorBlendFactor);
			attachment.dstColorBlendFactor = ToVk(attachmentInfo.m_DstColorBlendFactor);
			attachment.srcAlphaBlendFactor = ToVk(attachmentInfo.m_SrcAlphaBlendFactor);
			attachment.dstAlphaBlendFactor = ToVk(attachmentInfo.m_DstAlphaBlendFactor);
			attachment.colorBlendOp = ToVk(attachmentInfo.m_ColorBlendOp);
			attachment.alphaBlendOp = ToVk(attachmentInfo.m_AlphaBlendOp);
			attachment.colorWriteMask = ToVk(attachmentInfo.m_ColorMask);

		}

		VkPipelineColorBlendStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		vkInfo.attachmentCount = numColorAttachments;
		vkInfo.pAttachments = numColorAttachments <= 0 ? nullptr : colorAttachments.data();
		vkInfo.blendConstants[0] = info.m_BlendConstants.m_R;
		vkInfo.blendConstants[1] = info.m_BlendConstants.m_G;
		vkInfo.blendConstants[2] = info.m_BlendConstants.m_B;
		vkInfo.blendConstants[3] = info.m_BlendConstants.m_A;
		vkInfo.logicOpEnable = VK_FALSE;

		return vkInfo;

	}
	auto PipelineCreator::setup_multisample_info(const MultisamplePass& info) const noexcept -> VkPipelineMultisampleStateCreateInfo {

		VkPipelineMultisampleStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		vkInfo.alphaToOneEnable = info.m_AlphaToOne ? VK_TRUE : VK_FALSE;
		vkInfo.alphaToCoverageEnable = info.m_AlphaToCoverage ? VK_TRUE : VK_FALSE;
		vkInfo.rasterizationSamples = ToVk(info.m_SampleCount);

		return vkInfo;

	}
	auto PipelineCreator::setup_assembly_info(const AssemblyPass& info) const noexcept -> VkPipelineInputAssemblyStateCreateInfo {

		VkPipelineInputAssemblyStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		vkInfo.topology = ToVk(info.m_Topology);
		vkInfo.primitiveRestartEnable = info.m_PrimitiveRestart;

		return vkInfo;

	}
	auto PipelineCreator::setup_vertex_input_info(const VertexInputPass& info) const noexcept -> VkPipelineVertexInputStateCreateInfo {

		usize numAttributes = info.m_Attributes.size();
		static std::vector<VkVertexInputAttributeDescription> attributes{};
		attributes.clear();
		attributes.resize(numAttributes);

		for (uint32 i = 0; i < numAttributes; i++) {

			auto& vkAttr = attributes[i];
			const auto& attrInfo = info.m_Attributes[i];

			vkAttr.binding = attrInfo.m_Binding;
			vkAttr.format = ToVk(attrInfo.m_Format);
			vkAttr.location = attrInfo.m_Location;
			vkAttr.offset = attrInfo.m_Offset;

		}

		usize numBindings = info.m_Bindings.size();
		static std::vector<VkVertexInputBindingDescription> bindings{};
		bindings.clear();
		bindings.resize(numBindings);

		for (uint32 i = 0; i < numBindings; i++) {

			auto& vkBinding = bindings[i];
			const auto& bindingInfo = info.m_Bindings[i];

			vkBinding.binding = bindingInfo.m_Binding;
			vkBinding.inputRate = bindingInfo.m_Instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			vkBinding.stride = bindingInfo.m_Stride;

		}


		VkPipelineVertexInputStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vkInfo.vertexAttributeDescriptionCount = numAttributes;
		vkInfo.pVertexAttributeDescriptions = attributes.data();
		vkInfo.vertexBindingDescriptionCount = numBindings;
		vkInfo.pVertexBindingDescriptions = bindings.data();

		return vkInfo;

	}
	auto PipelineCreator::setup_shader_modules(const VkDevice logicalDevice, const PipelineCreateInfo& info) const noexcept -> std::vector<VkPipelineShaderStageCreateInfo> {

		usize numShaderInfos = info.m_ShaderInfos.size();
		LUM_ASSERT(numShaderInfos <= t_EnumCount<ShaderStage>, "Too much shader create infos in PipelineCreateInfo (MAX IS 5)");

		if (numShaderInfos == 0) {
			LUM_LOG_WARN("Couldn't create shader modules: There are 0 shader infos in pipeline.");
			return {};
		}

		std::array<bool, t_EnumCount<ShaderStage>> shaderSlots{};
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};

		for (auto& shaderInfo : info.m_ShaderInfos) {

			auto shaderIndex = ToUnderlyingEnum(shaderInfo.m_Stage);
			auto isOccupied = shaderSlots[shaderIndex];

			if (isOccupied) {
				LUM_LOG_WARN("Shader stage slot {} is already occupied, skipping shader '{}'", shaderIndex, shaderInfo.m_Path.ToString());
				continue;
			}

			VkPipelineShaderStageCreateInfo vkInfo{};
			vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vkInfo.pName = shaderInfo.m_EntryPoint.data();
			vkInfo.stage = ToVk(shaderInfo.m_Stage);
			vkInfo.module = create_shader_module(logicalDevice, shaderInfo.m_Path);

			shaderStages.push_back(vkInfo);

		}

		return shaderStages;

	}

	auto PipelineCreator::create_shader_module(const VkDevice logicalDevice, const Path& path) const noexcept -> VkShaderModule {

		auto binaryCodeResult = FileSystem::ReadBinaryFile(path);
		if (!binaryCodeResult) {
			LUM_LOG_ERROR("Couldn't create shader: {}!", binaryCodeResult.GetError());
			return VK_NULL_HANDLE;
		}
		auto& binary = binaryCodeResult.ValueRef();


		VkShaderModuleCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkInfo.pCode = reinterpret_cast<const uint32*>(binary.data());
		vkInfo.codeSize = binary.size() * sizeof(uint32);

		VkShaderModule module = VK_NULL_HANDLE;

		if (vkCreateShaderModule(logicalDevice, &vkInfo, nullptr, &module) != VK_SUCCESS) {
			LUM_LOG_ERROR("Couldn't create shader module! (Vulkan)");
			return VK_NULL_HANDLE;
		}

		return module;
	}

} // namespace lum::rhi::vk