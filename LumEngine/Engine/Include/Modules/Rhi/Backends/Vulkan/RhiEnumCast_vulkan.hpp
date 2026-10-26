/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <vulkan/vulkan_core.h>

#include "Core/CoreDefines.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi::vk {

	// Buffer enums conversions

	LUM_NODISCARD auto ToVk( BufferUsageFlags flags ) noexcept -> VkBufferUsageFlags;


	// Pipeline enums conversions

	LUM_NODISCARD auto ToVk( ShaderStage stage ) noexcept -> VkShaderStageFlagBits;
	LUM_NODISCARD auto ToVk( ShaderStageFlags flags ) noexcept -> VkShaderStageFlags;
	LUM_NODISCARD auto ToVk( PrimitiveTopology topology ) noexcept -> VkPrimitiveTopology;
	LUM_NODISCARD auto ToVk( CullMode mode ) noexcept -> VkCullModeFlags;
	LUM_NODISCARD auto ToVk( PolygonMode mode ) noexcept -> VkPolygonMode;
	LUM_NODISCARD auto ToVk( FrontFace face ) noexcept -> VkFrontFace;
	LUM_NODISCARD auto ToVk( BlendFactor factor ) noexcept -> VkBlendFactor;
	LUM_NODISCARD auto ToVk( BlendOp op ) noexcept -> VkBlendOp;
	LUM_NODISCARD auto ToVk( ColorComponentFlags flags ) noexcept -> VkColorComponentFlags;


	// Image enums conversions

	LUM_NODISCARD auto ToVk( ImageUsageFlags flags ) noexcept -> VkImageUsageFlags;
	LUM_NODISCARD auto ToVk( ImageFormat format ) noexcept -> VkFormat;
	LUM_NODISCARD auto ToVk( ImageDimension format ) noexcept -> VkImageType;
	LUM_NODISCARD auto ToVk( SampleCount samples ) noexcept -> VkSampleCountFlagBits;
	LUM_NODISCARD auto ToVk( ImageLayout layout ) noexcept -> VkImageLayout;

	LUM_NODISCARD auto GetAspectMask( ImageFormat format ) noexcept -> VkImageAspectFlags;


	// Sampler enums conversions

	LUM_NODISCARD auto ToVk( SamplerWrap wrap ) noexcept -> VkSamplerAddressMode;
	LUM_NODISCARD auto ToVk( SamplerFilter filter ) noexcept -> VkFilter;



	// Descriptor set enums conversions

	LUM_NODISCARD auto ToVk( ResourceType ) noexcept -> VkDescriptorType;


} // namespace lum::rhi::vk