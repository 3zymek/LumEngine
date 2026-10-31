#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header implementation file
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

#include "Core/Utils/Assert.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	// Buffer enums conversions

	auto ToVk( BufferUsageFlags flags ) noexcept -> VkBufferUsageFlags {

		VkBufferUsageFlags usage = 0;
		if (flags.Has( BufferUsage::Vertex )) {
			usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if (flags.Has( BufferUsage::Element )) {
			usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if (flags.Has( BufferUsage::Uniform )) {
			usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		if (flags.Has( BufferUsage::ShaderStorage )) {
			usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}
		if (flags.Has( BufferUsage::TransferSource )) {
			usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		if (flags.Has( BufferUsage::TransferDestination )) {
			usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		return usage;
	}



	auto ToVk( ShaderStage stage ) noexcept -> VkShaderStageFlagBits {
		switch (stage) {
			case ShaderStage::Vertex:   return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::Compute:  return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ShaderStage> );
		return VK_SHADER_STAGE_ALL;
	}
	auto ToVk( ShaderStageFlags flags ) noexcept -> VkShaderStageFlags {
		VkShaderStageFlags vkFlags = 0;

		if (flags.Has( ShaderStage::Vertex ))   vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;
		if (flags.Has( ShaderStage::Fragment )) vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (flags.Has( ShaderStage::Compute ))  vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;
		if (flags.Has( ShaderStage::Geometry )) vkFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;

		return vkFlags;
	}
	auto ToVk( PrimitiveTopology topology ) noexcept -> VkPrimitiveTopology {
		switch (topology) {
			case PrimitiveTopology::TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PrimitiveTopology::PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case PrimitiveTopology::LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveTopology::LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<PrimitiveTopology> );
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
	auto ToVk( CullMode mode ) noexcept -> VkCullModeFlags {
		switch (mode) {
			case CullMode::None:  return VK_CULL_MODE_NONE;
			case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:  return VK_CULL_MODE_BACK_BIT;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<CullMode> );
		return 0;
	}
	auto ToVk( PolygonMode mode ) noexcept -> VkPolygonMode {
		switch (mode) {
			case PolygonMode::Fill:      return VK_POLYGON_MODE_FILL;
			case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<PolygonMode> );
		return VK_POLYGON_MODE_MAX_ENUM;
	}
	auto ToVk( FrontFace face ) noexcept -> VkFrontFace {
		switch (face) {
			case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case FrontFace::Clockwise:        return VK_FRONT_FACE_CLOCKWISE;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<FrontFace> );
		return VK_FRONT_FACE_MAX_ENUM;
	}
	auto ToVk( BlendFactor factor ) noexcept -> VkBlendFactor {
		switch (factor) {
			case BlendFactor::Zero:                    return VK_BLEND_FACTOR_ZERO;
			case BlendFactor::One:                     return VK_BLEND_FACTOR_ONE;
			case BlendFactor::SrcColor:                return VK_BLEND_FACTOR_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor:        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DstColor:                return VK_BLEND_FACTOR_DST_COLOR;
			case BlendFactor::OneMinusDstColor:        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case BlendFactor::SrcAlpha:                return VK_BLEND_FACTOR_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha:        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha:                return VK_BLEND_FACTOR_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha:        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::ConstantColor:           return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor:   return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha:           return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha:   return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<BlendFactor> );
		return VK_BLEND_FACTOR_MAX_ENUM;
	}
	auto ToVk( BlendOp op ) noexcept -> VkBlendOp {
		switch (op) {
			case BlendOp::Add:              return VK_BLEND_OP_ADD;
			case BlendOp::Substract:        return VK_BLEND_OP_SUBTRACT;
			case BlendOp::ReverseSubstract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOp::Min:              return VK_BLEND_OP_MIN;
			case BlendOp::Max:              return VK_BLEND_OP_MAX;
		}
		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<BlendOp> );
		return VK_BLEND_OP_MAX_ENUM;
	}
	auto ToVk( ColorComponentFlags flags ) noexcept -> VkColorComponentFlags {
		VkColorComponentFlags vkFlags = 0;

		if (flags.Has( ColorComponentFlag::None )) return 0;
		if (flags.Has( ColorComponentFlag::Red ))    vkFlags |= VK_COLOR_COMPONENT_R_BIT;
		if (flags.Has( ColorComponentFlag::Green )) vkFlags |= VK_COLOR_COMPONENT_G_BIT;
		if (flags.Has( ColorComponentFlag::Blue ))  vkFlags |= VK_COLOR_COMPONENT_B_BIT;
		if (flags.Has( ColorComponentFlag::Alpha )) vkFlags |= VK_COLOR_COMPONENT_A_BIT;

		return vkFlags;
	}



	// Image enums

	auto ToVk( ImageUsageFlags flags ) noexcept -> VkImageUsageFlags {

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
	auto ToVk( ImageFormat format ) noexcept -> VkFormat {
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

			case ImageFormat::D16_UNORM: return VK_FORMAT_D16_UNORM;
			case ImageFormat::D32_FLOAT: return VK_FORMAT_D32_SFLOAT;

			case ImageFormat::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
			case ImageFormat::D32_FLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ImageFormat> );
		return VK_FORMAT_UNDEFINED;
	}
	auto ToVk( ImageDimension format ) noexcept -> VkImageType {

		switch (format) {
			case ImageDimension::Image1D: return VK_IMAGE_TYPE_1D;
			case ImageDimension::Image2D: return VK_IMAGE_TYPE_2D;
			case ImageDimension::Image3D: return VK_IMAGE_TYPE_3D;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ImageDimension> );
		return VK_IMAGE_TYPE_MAX_ENUM;

	}
	auto ToVk( SampleCount samples ) noexcept -> VkSampleCountFlagBits {
		switch (samples) {
			case SampleCount::Sample1:  return VK_SAMPLE_COUNT_1_BIT;
			case SampleCount::Sample2:  return VK_SAMPLE_COUNT_2_BIT;
			case SampleCount::Sample4:  return VK_SAMPLE_COUNT_4_BIT;
			case SampleCount::Sample8:  return VK_SAMPLE_COUNT_8_BIT;
			case SampleCount::Sample16: return VK_SAMPLE_COUNT_16_BIT;
			case SampleCount::Sample32: return VK_SAMPLE_COUNT_32_BIT;
			case SampleCount::Sample64: return VK_SAMPLE_COUNT_64_BIT;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<SampleCount> );
		return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
	}

	LUM_NODISCARD auto ToVk( ImageLayout layout ) noexcept -> VkImageLayout {
		switch (layout) {
			case ImageLayout::Undefined:                  return VK_IMAGE_LAYOUT_UNDEFINED;
			case ImageLayout::General:                    return VK_IMAGE_LAYOUT_GENERAL;
			case ImageLayout::ColorAttachment:            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case ImageLayout::DepthStencilAttachment:     return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case ImageLayout::DepthStencilReadOnly:       return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case ImageLayout::ShaderReadOnly:              return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case ImageLayout::TransferSource:              return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case ImageLayout::TransferDestination:         return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case ImageLayout::Present:                     return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ImageLayout> );
		return VK_IMAGE_LAYOUT_MAX_ENUM;
	}

	auto GetAspectMask( ImageFormat format ) noexcept -> VkImageAspectFlags {

		switch (format) {

			case ImageFormat::D16_UNORM:
			case ImageFormat::D32_FLOAT:
			return VK_IMAGE_ASPECT_DEPTH_BIT;

			case ImageFormat::D24_UNORM_S8_UINT:
			case ImageFormat::D32_FLOAT_S8_UINT:
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

			default:
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ImageFormat> );
		return {};

	}


	auto ToVk( SamplerWrap wrap ) noexcept -> VkSamplerAddressMode {
		switch (wrap) {
			case SamplerWrap::ClampBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			case SamplerWrap::ClampEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case SamplerWrap::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case SamplerWrap::RepeatMirrored: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<SamplerWrap> );
		return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
	}
	auto ToVk( SamplerFilter filter ) noexcept -> VkFilter {
		switch (filter) {
			case SamplerFilter::Linear: return VK_FILTER_LINEAR;
			case SamplerFilter::Nearest: return VK_FILTER_NEAREST;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<SamplerFilter> );
		return VK_FILTER_MAX_ENUM;
	}


	// Descriptor set enums conversions

	auto ToVk( ResourceType type ) noexcept -> VkDescriptorType {
		switch (type) {
			case ResourceType::Sampler:                return VK_DESCRIPTOR_TYPE_SAMPLER;
			case ResourceType::CombinedImageSampler:   return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case ResourceType::SampledImage:           return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			case ResourceType::StorageImage:           return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case ResourceType::UniformBuffer:          return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case ResourceType::StorageBuffer:          return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		}

		LUM_ASSERT( false, "Invalid {} enum", t_EnumTypeName<ResourceType> );
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}

} // namespace lum::rhi::vk