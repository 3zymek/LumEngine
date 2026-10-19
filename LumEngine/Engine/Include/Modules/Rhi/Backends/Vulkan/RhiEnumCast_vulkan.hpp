#pragma once

#include "Core/CoreDefines.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

#include <vulkan/vulkan.hpp>

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


	// Sampler enums conversions

	LUM_NODISCARD auto ToVk(SamplerWrap2 wrap) noexcept -> VkSamplerAddressMode;
	LUM_NODISCARD auto ToVk(SamplerFilter filter) noexcept -> VkFilter;

} // namespace lum::rhi::vk