#include "Rhi2/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

namespace lum::rhi::vk {

	constexpr auto ToVk( ShaderStage stage ) noexcept -> VkShaderStageFlagBits {
		switch (stage) {
			case ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::Compute:  return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		LUM_ASSERT( false, "Invalid ShaderStage" );
		return {};
	}
	constexpr auto ToVk( PrimitiveTopology topology ) noexcept -> VkPrimitiveTopology {
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
	constexpr auto ToVk( CullMode mode ) noexcept -> VkCullModeFlags {
		switch (mode) {
			case CullMode::None:  return VK_CULL_MODE_NONE;
			case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
		}
		LUM_ASSERT( false, "Invalid CullMode enum" );
		return {};
	}
	constexpr auto ToVk( PolygonMode mode ) noexcept -> VkPolygonMode {
		switch (mode) {
			case PolygonMode::Fill:      return VK_POLYGON_MODE_FILL;
			case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
		}
		LUM_ASSERT( false, "Invalid PolygonMode enum" );
		return {};
	}
	constexpr auto ToVk( FrontFace face ) noexcept -> VkFrontFace {
		switch (face) {
			case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case FrontFace::Clockwise:        return VK_FRONT_FACE_CLOCKWISE;
		}
		LUM_ASSERT( false, "Invalid FrontFace enum" );
		return {};
	}
	constexpr auto ToVk( BlendFactor factor ) noexcept -> VkBlendFactor {
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
	constexpr auto ToVk( BlendOp op ) noexcept -> VkBlendOp {
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
	constexpr auto ToVk( ColorComponentFlags flags ) noexcept -> VkColorComponentFlags {
		VkColorComponentFlags vkFlags = 0;

		if (flags.Has( ColorComponentFlag::None )) return 0;
		if (flags.Has( ColorComponentFlag::Red ))   vkFlags |= VK_COLOR_COMPONENT_R_BIT;
		if (flags.Has( ColorComponentFlag::Green )) vkFlags |= VK_COLOR_COMPONENT_G_BIT;
		if (flags.Has( ColorComponentFlag::Blue ))  vkFlags |= VK_COLOR_COMPONENT_B_BIT;
		if (flags.Has( ColorComponentFlag::Alpha )) vkFlags |= VK_COLOR_COMPONENT_A_BIT;

		return vkFlags;
	}



	// Image enums

	constexpr auto ToVk( ImageUsageFlags flags ) noexcept -> VkImageUsageFlags {

		VkImageUsageFlags result = 0;

		if (flags.Has( ImageUsageFlag::ColorAttachment )) {
			result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		if (flags.Has( ImageUsageFlag::DepthStencilAttachment )) {
			result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}
		if (flags.Has( ImageUsageFlag::InputAttachment )) {
			result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}
		if (flags.Has( ImageUsageFlag::ShaderReadable )) {
			result |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}
		if (flags.Has( ImageUsageFlag::ShaderReadWrite )) {
			result |= VK_IMAGE_USAGE_STORAGE_BIT;
		}
		if (flags.Has( ImageUsageFlag::TransferDestination )) {
			result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		if (flags.Has( ImageUsageFlag::TransferSource )) {
			result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}
		if (flags.Has( ImageUsageFlag::TransientAttachment )) {
			result |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		}
		
		return result;

	}
	constexpr auto ToVk( ImageFormat format ) noexcept -> VkFormat {
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
	constexpr auto ToVk( ImageType format ) noexcept -> VkImageType {

		switch (format) {
			case ImageType::Image1D: return VK_IMAGE_TYPE_1D;
			case ImageType::Image2D: return VK_IMAGE_TYPE_2D;
			case ImageType::Image3D: return VK_IMAGE_TYPE_3D;
		}

	}
	constexpr auto ToVk( SampleCount samples ) noexcept -> VkSampleCountFlagBits {
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

} // namespace lum::rhi::vk