#include "Rhi2/Backends/Vulkan/Utils/ImageCreator.hpp"
#include "Rhi2/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"

namespace lum::rhi::vk {

    //=======================================================//
    // Public
    //=======================================================//

    Result<VulkanImage> ImageCreator::CreateImage(const ImageCreateInfo2& info) noexcept {

        VkExtent3D extent{};
        extent.width = info.m_Extent.m_Width;
        extent.height = info.m_Extent.m_Height;
        extent.depth = info.m_Extent.m_Depth;

        VkFormat format = ToVk(info.m_Format);

        VkImageCreateInfo vkInfo{};
        vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        vkInfo.usage = ToVk(info.m_UsageFlags);
        vkInfo.format = format;
        vkInfo.imageType = ToVk(info.m_Dimension);
        vkInfo.extent = extent;
        vkInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        vkInfo.mipLevels = info.m_MipmapLevels;
        vkInfo.arrayLayers = info.m_ArrayLayers;
        vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkInfo.samples = ToVk(info.m_SampleCount);
        vkInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VulkanImage image{};

        const VkResult result = vkCreateImage(m_LogicalDevice, &vkInfo, nullptr, &image.m_Image);
        if (result != VK_SUCCESS) {
            return Result<VulkanImage>::Failure(
                FormatString("vkCreateImage failure (VkResult: {})", static_cast<int32>(result))
            );
        }

        VkMemoryRequirements req{};
        vkGetImageMemoryRequirements(m_LogicalDevice, image.m_Image, &req);

        uint32 memoryTypeIndex = m_Adapter().FindMemoryType(
            req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.memoryTypeIndex = memoryTypeIndex;
        allocInfo.allocationSize = req.size;

        vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &image.m_Memory);

        vkBindImageMemory(m_LogicalDevice, image.m_Image, image.m_Memory, 0);

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image.m_Image;
        viewInfo.viewType = choose_image_view_type(info);
        viewInfo.format = format;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = info.m_MipmapLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = info.m_ArrayLayers;
        viewInfo.subresourceRange.aspectMask = choose_aspect_mask(info.m_Format);

        const VkResult viewResult = vkCreateImageView(
            m_LogicalDevice, &viewInfo, nullptr, &image.m_View
        );

        if (viewResult != VK_SUCCESS) {
            return Result<VulkanImage>::Failure(
                FormatString("vkCreateImageView failure (VkResult: {})", static_cast<int32>(viewResult))
            );
        }

        return image;

    }

    Result<VulkanSampler> ImageCreator::CreateSampler(const SamplerCreateInfo2& info) noexcept {

        VkSamplerCreateInfo vkInfo{};
        vkInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        vkInfo.anisotropyEnable = (info.m_Anisotropy > 0.01f) ? VK_TRUE : VK_FALSE;
        vkInfo.addressModeU = ToVk(info.m_WrapU);
        vkInfo.addressModeV = ToVk(info.m_WrapV);
        vkInfo.addressModeW = ToVk(info.m_WrapW);
        vkInfo.minFilter = ToVk(info.m_MinFilter);
        vkInfo.mipmapMode = (info.m_MinFilter == SamplerFilter::Linear) ?
            VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;

        vkInfo.magFilter = ToVk(info.m_MagFilter);
        vkInfo.maxAnisotropy = info.m_MaxAnisotropy;


    }



    //=======================================================//
    // Private
    //=======================================================//

    auto ImageCreator::choose_image_view_type(const ImageCreateInfo2& info) noexcept -> VkImageViewType {

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
            return VK_IMAGE_VIEW_TYPE_CUBE;

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
        LUM_ASSERT(false, "Invalid ImageViewType enum");
        return {};
    }

    auto ImageCreator::choose_aspect_mask(ImageFormat format) noexcept -> VkImageAspectFlags {

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

    }

}