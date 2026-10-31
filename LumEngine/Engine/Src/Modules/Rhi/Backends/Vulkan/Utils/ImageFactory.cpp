
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
#include "Core/CorePch.hpp"
#include "Rhi/Backends/Vulkan/VulkanPch.hpp"

// Header impl file
#include "Rhi/Backends/Vulkan/Utils/ImageFactory.hpp"

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Assert.hpp"
#include "Core/Utils/Result.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiImage_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::vk {

	Result<VulkanImage> ImageResourceFactory::CreateImage( const VkDevice logicalDevice, const VulkanAdapter& adapter, const ImageCreateInfo& info ) const noexcept {

		if (logicalDevice == VK_NULL_HANDLE) {
			return Result<VulkanImage>::Failure(
				"CreateImage: Invalid logical device."
			);
		}

		const auto& extentInfo = info.m_Extent;

		if (extentInfo.m_Width == 0 ||
			extentInfo.m_Height == 0 ||
			extentInfo.m_Depth == 0 ||
			info.m_MipmapLevels == 0 ||
			info.m_ArrayLayers == 0) {
			return Result<VulkanImage>::Failure(
				"CreateImage: Invalid image dimensions, mip levels or array layers."
			);
		}

		if (info.m_UsageFlags == 0) {
			return Result<VulkanImage>::Failure(
				"CreateImage: Image usage flags cannot be empty."
			);
		}

		const VkFormat format = ToVk( info.m_Format );
		const VkImageType imageType = ToVk( info.m_Dimension );
		const VkSampleCountFlagBits sampleCount = ToVk( info.m_SampleCount );
		const VkImageViewType viewType = choose_image_view_type( info );

		uint32 maxDimension = std::max(
			extentInfo.m_Width,
			extentInfo.m_Height
		);

		if (imageType == VK_IMAGE_TYPE_3D) {
			maxDimension = std::max(
				maxDimension,
				extentInfo.m_Depth
			);
		}

		uint32 maxMipLevels = 1;

		while (maxDimension > 1) {
			maxDimension >>= 1;
			++maxMipLevels;
		}

		if (info.m_MipmapLevels > maxMipLevels) {
			return Result<VulkanImage>::Failure( "CreateImage: Mipmap level count exceeds image dimensions." );
		}

		if (info.m_MipmapLevels > 1 &&
			sampleCount != VK_SAMPLE_COUNT_1_BIT) {
			return Result<VulkanImage>::Failure( "CreateImage: Multisampled images cannot have multiple mip levels." );
		}

		if ((imageType != VK_IMAGE_TYPE_2D) && (sampleCount != VK_SAMPLE_COUNT_1_BIT)) {
			return Result<VulkanImage>::Failure( "CreateImage: Multisampling is only supported for 2D images." );
		}

		if ((viewType == VK_IMAGE_VIEW_TYPE_1D || viewType == VK_IMAGE_VIEW_TYPE_1D_ARRAY) &&
			imageType != VK_IMAGE_TYPE_1D) {
			return Result<VulkanImage>::Failure( "CreateImage: 1D view type requires a 1D image." );
		}

		if ((viewType == VK_IMAGE_VIEW_TYPE_2D ||
			  viewType == VK_IMAGE_VIEW_TYPE_2D_ARRAY ||
			  viewType == VK_IMAGE_VIEW_TYPE_CUBE ||
			  viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY) &&
			imageType != VK_IMAGE_TYPE_2D) {
			return Result<VulkanImage>::Failure( "CreateImage: 2D view type requires a 2D image." );
		}

		if (viewType == VK_IMAGE_VIEW_TYPE_3D &&
			imageType != VK_IMAGE_TYPE_3D) {
			return Result<VulkanImage>::Failure( "CreateImage: 3D view type requires a 3D image." );
		}

		VkImageCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkInfo.usage = ToVk( info.m_UsageFlags );
		vkInfo.format = format;
		vkInfo.imageType = imageType;
		vkInfo.extent = {
			extentInfo.m_Width,
			extentInfo.m_Height,
			extentInfo.m_Depth
		};
		vkInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		vkInfo.mipLevels = info.m_MipmapLevels;
		vkInfo.arrayLayers = info.m_ArrayLayers;
		vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfo.samples = sampleCount;
		vkInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VulkanImage image{};

		const auto failureCleanup = [ & ]( ) noexcept {

			if (image.m_VkView != VK_NULL_HANDLE) {
				vkDestroyImageView( logicalDevice, image.m_VkView, nullptr );
				image.m_VkView = VK_NULL_HANDLE;
			}

			if (image.m_VkImage != VK_NULL_HANDLE) {
				vkDestroyImage( logicalDevice, image.m_VkImage, nullptr );
				image.m_VkImage = VK_NULL_HANDLE;
			}

			if (image.m_VkMemory != VK_NULL_HANDLE) {
				vkFreeMemory( logicalDevice, image.m_VkMemory, nullptr );
				image.m_VkMemory = VK_NULL_HANDLE;
			}

			};

		ImageInfo imageInfo{};
		imageInfo.m_ArrayLayers = info.m_ArrayLayers;
		imageInfo.m_CurrentLayout = ImageLayout::Undefined;
		imageInfo.m_Extent = info.m_Extent;
		imageInfo.m_Format = info.m_Format;
		imageInfo.m_MipmapLevels = info.m_MipmapLevels;
		imageInfo.m_SampleCount = info.m_SampleCount;
		imageInfo.m_UsageFlags = info.m_UsageFlags;
		imageInfo.m_ViewType = info.m_ViewType;
		image.m_Info = imageInfo;

		VkResult result = vkCreateImage(
			logicalDevice,
			&vkInfo,
			nullptr,
			&image.m_VkImage
		);

		if (result != VK_SUCCESS) {
			return Result<VulkanImage>::Failure(
				FormatString( "vkCreateImage failure (VkResult: {})", static_cast<int32>(result) )
			);
		}

		VkMemoryRequirements requirements{};
		vkGetImageMemoryRequirements(
			logicalDevice,
			image.m_VkImage,
			&requirements
		);

		if (requirements.size == 0 ||
			requirements.memoryTypeBits == 0) {
			failureCleanup( );

			return Result<VulkanImage>::Failure( "CreateImage: Invalid image memory requirements." );
		}

		VkMemoryPropertyFlagBits property =
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		const uint32 memoryTypeIndex = adapter.FindMemoryType(
			requirements.memoryTypeBits,
			property
		);

		if (memoryTypeIndex >= 32 ||
			(requirements.memoryTypeBits & (1u << memoryTypeIndex)) == 0) {
			failureCleanup( );

			return Result<VulkanImage>::Failure( "CreateImage: No compatible memory type was found." );
		}

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		allocInfo.allocationSize = requirements.size;

		result = vkAllocateMemory(
			logicalDevice,
			&allocInfo,
			nullptr,
			&image.m_VkMemory
		);

		if (result != VK_SUCCESS) {
			failureCleanup( );

			return Result<VulkanImage>::Failure(
				FormatString( "vkAllocateMemory failure (VkResult: {})", static_cast<int32>(result) )
			);
		}

		result = vkBindImageMemory(
			logicalDevice,
			image.m_VkImage,
			image.m_VkMemory,
			0
		);

		if (result != VK_SUCCESS) {
			failureCleanup( );

			return Result<VulkanImage>::Failure(
				FormatString( "vkBindImageMemory failure (VkResult: {})", static_cast<int32>(result) )
			);
		}

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image.m_VkImage;
		viewInfo.viewType = viewType;
		viewInfo.format = format;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = info.m_MipmapLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = info.m_ArrayLayers;
		viewInfo.subresourceRange.aspectMask = GetAspectMask(
			info.m_Format
		);

		if (viewInfo.subresourceRange.aspectMask == 0) {
			failureCleanup( );

			return Result<VulkanImage>::Failure(
				"CreateImage: Invalid image aspect mask."
			);
		}

		const VkResult viewResult = vkCreateImageView(
			logicalDevice,
			&viewInfo,
			nullptr,
			&image.m_VkView
		);

		if (viewResult != VK_SUCCESS) {
			failureCleanup( );

			return Result<VulkanImage>::Failure(
				FormatString( "vkCreateImageView failure (VkResult: {})", static_cast<int32>(viewResult) )
			);
		}

		return image;

	}

	Result<VulkanSampler> ImageResourceFactory::CreateSampler( const VkDevice logicalDevice, const SamplerCreateInfo& info ) const noexcept {

		if (logicalDevice == VK_NULL_HANDLE) {
			return Result<VulkanSampler>::Failure( "CreateSampler: Invalid logical device." );
		}

		if (info.m_Anisotropy < 0.0f || info.m_MaxAnisotropy < 1.0f) {
			return Result<VulkanSampler>::Failure( "CreateSampler: Invalid anisotropy parameters." );
		}

		const bool anisotropyEnabled = info.m_Anisotropy > 0.01f;

		VkSamplerCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		vkInfo.anisotropyEnable = anisotropyEnabled ? VK_TRUE : VK_FALSE;
		vkInfo.maxAnisotropy = anisotropyEnabled
			? info.m_MaxAnisotropy
			: 1.0f;
		vkInfo.addressModeU = ToVk( info.m_WrapU );
		vkInfo.addressModeV = ToVk( info.m_WrapV );
		vkInfo.addressModeW = ToVk( info.m_WrapW );
		vkInfo.minFilter = ToVk( info.m_MinFilter );
		vkInfo.mipmapMode = info.m_MinFilter == SamplerFilter::Linear
			? VK_SAMPLER_MIPMAP_MODE_LINEAR
			: VK_SAMPLER_MIPMAP_MODE_NEAREST;
		vkInfo.magFilter = ToVk( info.m_MagFilter );
		vkInfo.minLod = 0.0f;
		vkInfo.maxLod = VK_LOD_CLAMP_NONE;
		vkInfo.mipLodBias = 0.0f;
		vkInfo.compareEnable = VK_FALSE;
		vkInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		vkInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		vkInfo.unnormalizedCoordinates = VK_FALSE;

		VulkanSampler sampler{};

		const VkResult result = vkCreateSampler(
			logicalDevice,
			&vkInfo,
			nullptr,
			&sampler.m_VkSampler 
		);

		if (result != VK_SUCCESS) {
			return Result<VulkanSampler>::Failure( 
				FormatString( "vkCreateSampler failure (VkResult: {})", static_cast<int32>(result) ) 
			);
		}

		return sampler;

	}

	auto ImageResourceFactory::choose_image_view_type( const ImageCreateInfo& info ) const noexcept -> VkImageViewType {

		switch (info.m_ViewType) {

			case ImageViewType::Default:
			switch (info.m_Dimension) {
				case ImageDimension::Image1D:
				return VK_IMAGE_VIEW_TYPE_1D;

				case ImageDimension::Image2D:
				return VK_IMAGE_VIEW_TYPE_2D;

				case ImageDimension::Image3D:
				return VK_IMAGE_VIEW_TYPE_3D;
			}
			break;

			case ImageViewType::ImageCube:
			if (info.m_ArrayLayers == 6) {
				return VK_IMAGE_VIEW_TYPE_CUBE;
			}
			if (info.m_ArrayLayers > 6 &&
				info.m_ArrayLayers % 6 == 0) {
				return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			}
			break;

			case ImageViewType::ImageArray:
			switch (info.m_Dimension) {
				case ImageDimension::Image1D:
				return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

				case ImageDimension::Image2D:
				return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

				case ImageDimension::Image3D:
				break;
			}
			break;
		}

		LUM_ASSERT( false, "Invalid ImageViewType enum" );
		return VK_IMAGE_VIEW_TYPE_MAX_ENUM;

	}

} // namespace lum::rhi::vk
