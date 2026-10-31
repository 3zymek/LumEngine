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

#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiResourceSet_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiTypes_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi::vk {

	class VulkanDevice;

	class ResourceSetFactory {
	public:

		Result<VulkanResourceLayout> CreateResourceLayout( VkDevice logicalDevice, const ResourceLayoutCreateInfo& info );
		Result<VulkanResourceSet> CreateResourceSet( VulkanDevice& device, const ResourceSetCreateInfo& info );

	};


} // namespace lum::rhi