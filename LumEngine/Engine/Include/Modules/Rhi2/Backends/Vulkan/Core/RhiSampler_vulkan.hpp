#pragma once

#include "Rhi2/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::vk {

    struct VulkanSampler {

        VkSampler m_Sampler = VK_NULL_HANDLE;

    };

}