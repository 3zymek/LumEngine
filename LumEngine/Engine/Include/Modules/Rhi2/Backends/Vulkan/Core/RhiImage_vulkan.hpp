#pragma once
#include "Rhi2/Interfaces/Core/RhiImage.hpp"

namespace lum::rhi::vk {

    struct VulkanImage {

        VkImage m_Image{};
        VkImageView m_ImageView{};

    };

} // namespace lum::rhi::vk