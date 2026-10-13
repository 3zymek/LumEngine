#pragma once
#include "Rhi2/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiImage_vulkan.hpp"

namespace lum::rhi::vk {

	// Pipeline enums conversions

	LUM_NODISCARD auto ToVk(ShaderStage stage) noexcept -> VkShaderStageFlagBits;
	LUM_NODISCARD auto ToVk(PrimitiveTopology topology) noexcept -> VkPrimitiveTopology;
	LUM_NODISCARD auto ToVk(CullMode mode) noexcept -> VkCullModeFlags;
	LUM_NODISCARD auto ToVk(PolygonMode mode) noexcept -> VkPolygonMode;
	LUM_NODISCARD auto ToVk(FrontFace face) noexcept -> VkFrontFace;
	LUM_NODISCARD auto ToVk(BlendFactor factor) noexcept -> VkBlendFactor;
	LUM_NODISCARD auto ToVk(BlendOp op) noexcept -> VkBlendOp;
	LUM_NODISCARD auto ToVk(ColorComponentFlags flags) noexcept -> VkColorComponentFlags;


	// Image enums conversions

	LUM_NODISCARD auto ToVk(ImageUsageFlags flags) noexcept -> VkImageUsageFlags;
	LUM_NODISCARD auto ToVk(ImageFormat format) noexcept -> VkFormat;
	LUM_NODISCARD auto ToVk(ImageDimension format) noexcept -> VkImageType;
	LUM_NODISCARD auto ToVk(SampleCount samples) noexcept -> VkSampleCountFlagBits;
	LUM_NODISCARD auto ToVk(ImageViewType type) noexcept -> VkImageViewType;

} // namespace lum::rhi::vk