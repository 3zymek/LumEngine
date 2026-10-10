#include "Rhi2/Backends/Vulkan/Utils/PipelineCreator.hpp"
#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"

namespace lum::rhi::vk {

	//=======================================================//
	// Public
	//=======================================================//

	Result<VulkanPipeline> PipelineCreator::CreatePipeline( const PipelineCreateInfo2& info ) {
		
		VulkanPipeline pipeline{};

		static constexpr std::array<VkDynamicState, 2> s_DynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = SafeCast<uint32>( s_DynamicStates.size( ) );
		dynamicState.pDynamicStates = s_DynamicStates.data( );

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;

		VkGraphicsPipelineCreateInfo pipInfo{};
		pipInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipInfo.pDynamicState = &dynamicState;

		VkSurfaceFormatKHR surfaceFormat = m_Adapter().m_SurfaceSupport.SelectSurfaceFormat( );
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
				m_LogicalDevice, 
				&layoutInfo, 
				nullptr, 
				&pipeline.m_Layout 
			);

		if (layoutResult != VK_SUCCESS) {
			return Result<VulkanPipeline>::Failure(
				FormatString( "Failed to create pipeline layout! (VkResult: {})", static_cast<int32>(layoutResult) ) 
			);
		}

		auto assemblyState = setup_assembly_info( info.m_AssemblyPass );
		pipInfo.pInputAssemblyState = &assemblyState;

		auto rasterizationState = setup_rasterizer_info( info.m_RasterizationPass );
		pipInfo.pRasterizationState = &rasterizationState;

		auto colorBlendState = setup_color_blend_info( info.m_ColorBlendPass );
		pipInfo.pColorBlendState = &colorBlendState;

		auto msState = setup_multisample_info( info.m_MultisamplePass );
		pipInfo.pMultisampleState = &msState;

		auto vertexInputState = setup_vertex_input_info( info.m_VertexInputPass );
		pipInfo.pVertexInputState = &vertexInputState;

		auto shaderStages = setup_shader_modules( info );
		pipInfo.stageCount = shaderStages.size( );
		pipInfo.pStages = shaderStages.data( );

		pipInfo.pNext = &renderingInfo;
		pipInfo.pViewportState = &viewportState;
		pipInfo.layout = pipeline.m_Layout;

		const VkResult pipResult = 
			vkCreateGraphicsPipelines( 
				m_LogicalDevice, 
				VK_NULL_HANDLE, 
				1, 
				&pipInfo, 
				nullptr, 
				&pipeline.m_Pipeline 
			);

		if (pipResult != VK_SUCCESS) {
			return Result<VulkanPipeline>::Failure( 
				FormatString( "Failed to create graphics pipeline! (VkResult: {})", static_cast<int32>(pipResult) )
			);
		}

		return pipeline;

	}




	//=======================================================//
	// Private
	//=======================================================//

	auto PipelineCreator::setup_rasterizer_info( const RasterizationPass& info ) noexcept -> VkPipelineRasterizationStateCreateInfo {

		VkPipelineRasterizationStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		vkInfo.rasterizerDiscardEnable = VK_FALSE;
		vkInfo.cullMode = to_vk( info.m_CullMode );
		vkInfo.frontFace = to_vk( info.m_FrontFace );
		vkInfo.polygonMode = to_vk( info.m_PolygonMode );
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
	auto PipelineCreator::setup_color_blend_info( const ColorBlendPass& info ) noexcept -> VkPipelineColorBlendStateCreateInfo {

		const uint32 numColorAttachments = SafeCast<uint32>( info.m_Attachments.size( ) );
		std::vector<VkPipelineColorBlendAttachmentState> colorAttachments( numColorAttachments );
		for (uint32 i = 0; i < numColorAttachments; i++) {

			auto& attachment = colorAttachments[ i ];
			const auto& attachmentInfo = info.m_Attachments[ i ];
			attachment.blendEnable = attachmentInfo.m_BlendEnabled ? VK_TRUE : VK_FALSE;
			attachment.srcColorBlendFactor = to_vk( attachmentInfo.m_SrcColorBlendFactor );
			attachment.dstColorBlendFactor = to_vk( attachmentInfo.m_DstColorBlendFactor );
			attachment.srcAlphaBlendFactor = to_vk( attachmentInfo.m_SrcAlphaBlendFactor );
			attachment.dstAlphaBlendFactor = to_vk( attachmentInfo.m_DstAlphaBlendFactor );
			attachment.colorBlendOp = to_vk( attachmentInfo.m_ColorBlendOp );
			attachment.alphaBlendOp = to_vk( attachmentInfo.m_AlphaBlendOp );
			attachment.colorWriteMask = to_vk( attachmentInfo.m_ColorMask );

		}

		VkPipelineColorBlendStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		vkInfo.attachmentCount = numColorAttachments;
		vkInfo.pAttachments = numColorAttachments <= 0 ? nullptr : colorAttachments.data( );
		vkInfo.blendConstants[ 0 ] = info.m_BlendConstants.m_R;
		vkInfo.blendConstants[ 1 ] = info.m_BlendConstants.m_G;
		vkInfo.blendConstants[ 2 ] = info.m_BlendConstants.m_B;
		vkInfo.blendConstants[ 3 ] = info.m_BlendConstants.m_A;
		vkInfo.logicOpEnable = VK_FALSE;

		return vkInfo;

	}
	auto PipelineCreator::setup_multisample_info( const MultisamplePass& info ) noexcept -> VkPipelineMultisampleStateCreateInfo {

		VkPipelineMultisampleStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		vkInfo.alphaToOneEnable = info.m_AlphaToOne ? VK_TRUE : VK_FALSE;
		vkInfo.alphaToCoverageEnable = info.m_AlphaToCoverage ? VK_TRUE : VK_FALSE;
		vkInfo.rasterizationSamples = to_vk( info.m_SampleCount );

		return vkInfo;

	}
	auto PipelineCreator::setup_assembly_info( const AssemblyPass& info ) noexcept -> VkPipelineInputAssemblyStateCreateInfo {

		VkPipelineInputAssemblyStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		vkInfo.topology = to_vk( info.m_Topology );
		vkInfo.primitiveRestartEnable = info.m_PrimitiveRestart;

		return vkInfo;

	}
	auto PipelineCreator::setup_vertex_input_info( const VertexInputPass& info ) noexcept -> VkPipelineVertexInputStateCreateInfo {

		usize numAttributes = info.m_Attributes.size( );
		std::vector<VkVertexInputAttributeDescription> attributes( numAttributes );
		for (uint32 i = 0; i < numAttributes; i++) {

			auto& vkAttr = attributes[ i ];
			const auto& attrInfo = info.m_Attributes[ i ];

			vkAttr.binding = attrInfo.m_Binding;
			vkAttr.format = to_vk( attrInfo.m_Format );
			vkAttr.location = attrInfo.m_Location;
			vkAttr.offset = attrInfo.m_Offset;

		}

		usize numBindings = info.m_Bindings.size( );
		std::vector<VkVertexInputBindingDescription> bindings( numBindings );
		for (uint32 i = 0; i < numBindings; i++) {

			auto& vkBinding = bindings[ i ];
			const auto& bindingInfo = info.m_Bindings[ i ];

			vkBinding.binding = bindingInfo.m_Binding;
			vkBinding.inputRate = bindingInfo.m_Instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			vkBinding.stride = bindingInfo.m_Stride;

		}


		VkPipelineVertexInputStateCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vkInfo.vertexAttributeDescriptionCount = numAttributes;
		vkInfo.pVertexAttributeDescriptions = attributes.data( );
		vkInfo.vertexBindingDescriptionCount = numBindings;
		vkInfo.pVertexBindingDescriptions = bindings.data( );

		return vkInfo;

	}
	auto PipelineCreator::setup_shader_modules( const PipelineCreateInfo2& info ) noexcept -> std::vector<VkPipelineShaderStageCreateInfo> {

		LUM_ASSERT( info.m_ShaderInfos.size( ) <= t_EnumCount<ShaderStage>, "Too much shader create infos in PipelineCreateInfo (MAX IS 5)" );

		std::array<bool, t_EnumCount<ShaderStage>> shaderSlots{};
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};

		for (auto& shaderInfo : info.m_ShaderInfos) {

			auto shaderIndex = ToUnderlyingEnum( shaderInfo.m_Stage );
			auto isOccupied = shaderSlots[ shaderIndex ];

			if (isOccupied) {
				LUM_LOG_WARN( "Shader stage slot {} is already occupied, skipping shader '{}'", shaderIndex, shaderInfo.m_Path.ToString( ) );
				continue;
			}

			VkPipelineShaderStageCreateInfo vkInfo{};
			vkInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vkInfo.pName = shaderInfo.m_EntryPoint.data( );
			vkInfo.stage = to_vk( shaderInfo.m_Stage );
			vkInfo.module = create_shader_module( shaderInfo.m_Path );

		}

		return shaderStages;

	}

	auto PipelineCreator::create_shader_module( const Path& path ) noexcept -> VkShaderModule {
		auto binaryCode = FileSystem::ReadBinaryFile( path );
		if (!binaryCode) {
			LUM_LOG_ERROR( "Failed to create shader: {}!", binaryCode.GetError( ) );
			return VK_NULL_HANDLE;
		}

		VkShaderModuleCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkInfo.pCode = reinterpret_cast<const uint32_t*>(binaryCode->data( ));
		vkInfo.codeSize = binaryCode->size( );

		VkShaderModule module = VK_NULL_HANDLE;

		if (vkCreateShaderModule( m_LogicalDevice, &vkInfo, nullptr, &module ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Failed to create shader module! (Vulkan)" );
			return VK_NULL_HANDLE;
		}

		return module;
	}

	constexpr auto PipelineCreator::to_vk( ShaderStage stage ) noexcept -> VkShaderStageFlagBits {
		switch (stage) {
			case ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::Compute:  return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		LUM_ASSERT( false, "Invalid ShaderStage" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( PrimitiveTopology topology ) noexcept -> VkPrimitiveTopology {
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
	constexpr auto PipelineCreator::to_vk( CullMode mode ) noexcept -> VkCullModeFlags {
		switch (mode) {
			case CullMode::None:  return VK_CULL_MODE_NONE;
			case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
		}
		LUM_ASSERT( false, "Invalid CullMode enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( PolygonMode mode ) noexcept -> VkPolygonMode {
		switch (mode) {
			case PolygonMode::Fill:      return VK_POLYGON_MODE_FILL;
			case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
		}
		LUM_ASSERT( false, "Invalid PolygonMode enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( FrontFace face ) noexcept -> VkFrontFace {
		switch (face) {
			case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case FrontFace::Clockwise:        return VK_FRONT_FACE_CLOCKWISE;
		}
		LUM_ASSERT( false, "Invalid FrontFace enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( BlendFactor factor ) noexcept -> VkBlendFactor {
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
		LUM_ASSERT( false, "Invalid BlendFactor enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( BlendOp op ) noexcept -> VkBlendOp {
		switch (op) {
			case BlendOp::Add:              return VK_BLEND_OP_ADD;
			case BlendOp::Substract:        return VK_BLEND_OP_SUBTRACT;
			case BlendOp::ReverseSubstract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOp::Min:              return VK_BLEND_OP_MIN;
			case BlendOp::Max:              return VK_BLEND_OP_MAX;
		}
		LUM_ASSERT( false, "Invalid BlendOp enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( Flags<ColorComponentFlag> flags ) noexcept -> VkColorComponentFlags {
		VkColorComponentFlags vkFlags = 0;

		if (flags.Has( ColorComponentFlag::None )) return 0;
		if (flags.Has( ColorComponentFlag::Red ))   vkFlags |= VK_COLOR_COMPONENT_R_BIT;
		if (flags.Has( ColorComponentFlag::Green )) vkFlags |= VK_COLOR_COMPONENT_G_BIT;
		if (flags.Has( ColorComponentFlag::Blue ))  vkFlags |= VK_COLOR_COMPONENT_B_BIT;
		if (flags.Has( ColorComponentFlag::Alpha )) vkFlags |= VK_COLOR_COMPONENT_A_BIT;

		return vkFlags;
	}
	constexpr auto PipelineCreator::to_vk( SampleCount samples ) noexcept -> VkSampleCountFlagBits {
		switch (samples) {
			case SampleCount::Sample1:  return VK_SAMPLE_COUNT_1_BIT;
			case SampleCount::Sample2:  return VK_SAMPLE_COUNT_2_BIT;
			case SampleCount::Sample4:  return VK_SAMPLE_COUNT_4_BIT;
			case SampleCount::Sample8:  return VK_SAMPLE_COUNT_8_BIT;
			case SampleCount::Sample16: return VK_SAMPLE_COUNT_16_BIT;
			case SampleCount::Sample32: return VK_SAMPLE_COUNT_32_BIT;
			case SampleCount::Sample64: return VK_SAMPLE_COUNT_64_BIT;
		}

		LUM_ASSERT( false, "Invalid SampleCount enum" );
		return {};
	}
	constexpr auto PipelineCreator::to_vk( ImageFormat format ) noexcept -> VkFormat {
		switch (format) {
			case ImageFormat::R8_UNORM:     return VK_FORMAT_R8_UNORM;
			case ImageFormat::R8_SNORM:     return VK_FORMAT_R8_SNORM;
			case ImageFormat::R8_UINT:      return VK_FORMAT_R8_UINT;
			case ImageFormat::R8_SINT:      return VK_FORMAT_R8_SINT;
			case ImageFormat::R8_SRGB:      return VK_FORMAT_R8_SRGB;

			case ImageFormat::RG8_UNORM:    return VK_FORMAT_R8G8_UNORM;
			case ImageFormat::RG8_SNORM:    return VK_FORMAT_R8G8_SNORM;
			case ImageFormat::RG8_UINT:     return VK_FORMAT_R8G8_UINT;
			case ImageFormat::RG8_SINT:     return VK_FORMAT_R8G8_SINT;
			case ImageFormat::RG8_SRGB:     return VK_FORMAT_R8G8_SRGB;

			case ImageFormat::RGB8_UNORM:   return VK_FORMAT_R8G8B8_UNORM;
			case ImageFormat::RGB8_SNORM:   return VK_FORMAT_R8G8B8_SNORM;
			case ImageFormat::RGB8_UINT:    return VK_FORMAT_R8G8B8_UINT;
			case ImageFormat::RGB8_SINT:    return VK_FORMAT_R8G8B8_SINT;
			case ImageFormat::RGB8_SRGB:    return VK_FORMAT_R8G8B8_SRGB;

			case ImageFormat::RGBA8_UNORM:  return VK_FORMAT_R8G8B8A8_UNORM;
			case ImageFormat::RGBA8_SNORM:  return VK_FORMAT_R8G8B8A8_SNORM;
			case ImageFormat::RGBA8_UINT:   return VK_FORMAT_R8G8B8A8_UINT;
			case ImageFormat::RGBA8_SINT:   return VK_FORMAT_R8G8B8A8_SINT;
			case ImageFormat::RGBA8_SRGB:   return VK_FORMAT_R8G8B8A8_SRGB;

			case ImageFormat::R16_UNORM:    return VK_FORMAT_R16_UNORM;
			case ImageFormat::R16_SNORM:    return VK_FORMAT_R16_SNORM;
			case ImageFormat::R16_UINT:     return VK_FORMAT_R16_UINT;
			case ImageFormat::R16_SINT:     return VK_FORMAT_R16_SINT;
			case ImageFormat::R16_FLOAT:    return VK_FORMAT_R16_SFLOAT;

			case ImageFormat::RG16_UNORM:   return VK_FORMAT_R16G16_UNORM;
			case ImageFormat::RG16_SNORM:   return VK_FORMAT_R16G16_SNORM;
			case ImageFormat::RG16_UINT:    return VK_FORMAT_R16G16_UINT;
			case ImageFormat::RG16_SINT:    return VK_FORMAT_R16G16_SINT;
			case ImageFormat::RG16_FLOAT:   return VK_FORMAT_R16G16_SFLOAT;

			case ImageFormat::RGB16_UNORM:  return VK_FORMAT_R16G16B16_UNORM;
			case ImageFormat::RGB16_SNORM:  return VK_FORMAT_R16G16B16_SNORM;
			case ImageFormat::RGB16_UINT:   return VK_FORMAT_R16G16B16_UINT;
			case ImageFormat::RGB16_SINT:   return VK_FORMAT_R16G16B16_SINT;
			case ImageFormat::RGB16_FLOAT:  return VK_FORMAT_R16G16B16_SFLOAT;

			case ImageFormat::RGBA16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
			case ImageFormat::RGBA16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
			case ImageFormat::RGBA16_UINT:  return VK_FORMAT_R16G16B16A16_UINT;
			case ImageFormat::RGBA16_SINT:  return VK_FORMAT_R16G16B16A16_SINT;
			case ImageFormat::RGBA16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;

			case ImageFormat::R32_UINT:     return VK_FORMAT_R32_UINT;
			case ImageFormat::R32_SINT:     return VK_FORMAT_R32_SINT;
			case ImageFormat::R32_FLOAT:    return VK_FORMAT_R32_SFLOAT;

			case ImageFormat::RG32_UINT:    return VK_FORMAT_R32G32_UINT;
			case ImageFormat::RG32_SINT:    return VK_FORMAT_R32G32_SINT;
			case ImageFormat::RG32_FLOAT:   return VK_FORMAT_R32G32_SFLOAT;

			case ImageFormat::RGB32_UINT:   return VK_FORMAT_R32G32B32_UINT;
			case ImageFormat::RGB32_SINT:   return VK_FORMAT_R32G32B32_SINT;
			case ImageFormat::RGB32_FLOAT:  return VK_FORMAT_R32G32B32_SFLOAT;

			case ImageFormat::RGBA32_UINT:  return VK_FORMAT_R32G32B32A32_UINT;
			case ImageFormat::RGBA32_SINT:  return VK_FORMAT_R32G32B32A32_SINT;
			case ImageFormat::RGBA32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
		}

		LUM_ASSERT( false, "Invalid ImageFormat enum" );
		return VK_FORMAT_UNDEFINED;
	}

} // namespace lum::rhi::vk