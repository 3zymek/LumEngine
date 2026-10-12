#pragma once
#include "Rhi2/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiImage_vulkan.hpp"

namespace lum::rhi::vk {

	// Pipeline enums conversions

	LUM_NODISCARD constexpr auto ToVk( ShaderStage stage ) noexcept -> VkShaderStageFlagBits;
	LUM_NODISCARD constexpr auto ToVk( PrimitiveTopology topology ) noexcept -> VkPrimitiveTopology;
	LUM_NODISCARD constexpr auto ToVk( CullMode mode ) noexcept -> VkCullModeFlags;
	LUM_NODISCARD constexpr auto ToVk( PolygonMode mode ) noexcept -> VkPolygonMode;
	LUM_NODISCARD constexpr auto ToVk( FrontFace face ) noexcept -> VkFrontFace;
	LUM_NODISCARD constexpr auto ToVk( BlendFactor factor ) noexcept -> VkBlendFactor;
	LUM_NODISCARD constexpr auto ToVk( BlendOp op ) noexcept -> VkBlendOp;
	LUM_NODISCARD constexpr auto ToVk( ColorComponentFlags flags ) noexcept -> VkColorComponentFlags;


	// Image enums conversions

	LUM_NODISCARD constexpr auto ToVk( ImageUsageFlags flags ) noexcept -> VkImageUsageFlags;
	LUM_NODISCARD constexpr auto ToVk( ImageFormat format ) noexcept -> VkFormat;
	LUM_NODISCARD constexpr auto ToVk( ImageType format ) noexcept -> VkImageType;
	LUM_NODISCARD constexpr auto ToVk( SampleCount samples ) noexcept -> VkSampleCountFlagBits;

} // namespace lum::rhi::vk