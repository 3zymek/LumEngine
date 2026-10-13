#pragma once

#include "Rhi2/Backends/Vulkan/Core/RhiImage_vulkan.hpp"

namespace lum::rhi::vk {

    class ImageCreator {
    public:

        void Initialize(VkDevice logicalDevice) {
            m_LogicalDevice = logicalDevice;
        }

        Result<VulkanImage> CreateImage(const ImageCreateinfo2& info) noexcept {

            VkExtent3D extent{};
            extent.width = info.m_Extent.m_Width;
            extent.height = info.m_Extent.m_Height;
            extent.depth = info.m_Extent.m_Depth;

            VkImageCreateInfo vkInfo{};
            vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            vkInfo.usage = ToVk(info.m_UsageFlags);
            vkInfo.format = ToVk(info.m_Format);
            vkInfo.imageType = ToVk(info.m_Type);
            vkInfo.extent = extent;
            vkInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            vkInfo.mipLevels = info.m_MipmapLevels;
            vkInfo.arrayLayers = info.m_ArrayLayers;
            vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            vkInfo.initialLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

            VulkanImage image{};

            const VkResult result = vkCreateImage(m_LogicalDevice, &vkInfo, nullptr, &image.m_Image);
            if (result != VK_SUCCESS) {
                LUM_LOG_ERROR("Couldn't create image: (VkResult: {})", static_cast<uint32>(result));
                return {};
            }

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image.m_Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;


            auto handle = m_Images.Append(image);

        }



    private:

        VkDevice m_LogicalDevice = VK_NULL_HANDLE;

        VkImageViewType choose_image_view_type(const ImageCreateInfo& info) {

            switch (info.viewType) {

            case ImageViewType::Default:
                switch (info.dimension) {
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
                switch (info.dimension) {
                case ImageDimension::Image1D:
                    return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

                case ImageDimension::Image2D:
                    return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

                case ImageDimension::Image3D:
                    break;
                }
                break;
            }

            return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
        }

    };

}