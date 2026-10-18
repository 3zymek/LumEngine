#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "Core/Utils/Result.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiImage_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
#include "Rhi2/Interfaces/Core/RhiImage.hpp"
#include "Rhi2/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::vk {

	class VulkanAdapter;

	class ImageCreator {
	public:

		void Initialize( VkDevice logicalDevice, SafePtr<VulkanAdapter> adapter ) {
			m_LogicalDevice = logicalDevice;
			m_Adapter = adapter;
		}

		Result<VulkanImage> CreateImage( const ImageCreateInfo2& info ) noexcept;
		Result<VulkanSampler> CreateSampler( const SamplerCreateInfo2& info ) noexcept;

	private:

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		SafePtr<VulkanAdapter> m_Adapter = nullptr;

		auto choose_image_view_type( const ImageCreateInfo2& info ) noexcept -> VkImageViewType;
		auto choose_aspect_mask( ImageFormat format ) noexcept -> VkImageAspectFlags;

	};

}