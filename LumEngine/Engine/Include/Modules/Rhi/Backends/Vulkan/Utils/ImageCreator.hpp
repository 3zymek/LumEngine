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

#include <Core/Utils/Result.hpp>
#include <Rhi/Backends/Vulkan/Core/RhiImage_vulkan.hpp>
#include <Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp>
#include <Rhi/Interfaces/Core/RhiSampler.hpp>
#include <Rhi/Interfaces/Core/RhiImage.hpp>


namespace lum::rhi::vk {

	class VulkanAdapter;

	class ImageResourceCreator {
	public:

		Result<VulkanImage> CreateImage(const VkDevice logicalDevice, const VulkanAdapter& adapter, const ImageCreateInfo& info) const noexcept;
		Result<VulkanSampler> CreateSampler(const VkDevice logicalDevice, const SamplerCreateInfo& info) const noexcept;

	private:

		auto choose_image_view_type(const ImageCreateInfo& info) const noexcept -> VkImageViewType;

	};

}