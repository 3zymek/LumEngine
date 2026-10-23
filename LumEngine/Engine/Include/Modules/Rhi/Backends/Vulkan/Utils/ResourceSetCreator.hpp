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
#include <volk.h>

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiResourceSet_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiTypes_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"


namespace lum::rhi::vk {


	class ResourceSetCreator {
	public:

		Result<VulkanResourceSet> CreateResourceSet( VkDevice logicalDevice, const ResourceSetCreateInfo& info ) {

			const usize bindingsCount = info.m_Bindings.size( );

			std::vector<VkDescriptorSetLayoutBinding> bindings( bindingsCount );
			std::vector<VkDescriptorPoolSize> poolSizes( bindingsCount );

			for (uint32 i = 0; i < bindingsCount; i++) {

				auto& vkInfo = bindings[ i ];
				auto& nativeInfo = info.m_Bindings[ i ];

				auto resourceType = ToVk( nativeInfo.m_Type );

				vkInfo.descriptorType = resourceType;
				vkInfo.binding = nativeInfo.m_Binding;
				vkInfo.descriptorCount = nativeInfo.m_ResourceCount;
				vkInfo.stageFlags = ToVk( nativeInfo.m_ShaderStages );
				vkInfo.pImmutableSamplers = nullptr;

				auto& pool = poolSizes[ i ];
				pool.descriptorCount = nativeInfo.m_ResourceCount;
				pool.type = resourceType;

			}

			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = bindingsCount;
			layoutInfo.pBindings = bindings.data( );

			VulkanResourceSet resourceSet{};

			const VkResult layoutResult = vkCreateDescriptorSetLayout( logicalDevice, &layoutInfo, nullptr, &resourceSet.m_Layout );
			if (layoutResult != VK_SUCCESS) {
				return Result<VulkanResourceSet>::Failure(
					FormatString( "vkCreateDescriptorSetLayout failure (VkResult: {})", static_cast<int32>(layoutResult) )
				);
			}

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.maxSets = 1;
			poolInfo.poolSizeCount = poolSizes.size( );
			poolInfo.pPoolSizes = poolSizes.data( );

			const VkResult poolResult = vkCreateDescriptorPool( logicalDevice, &poolInfo, nullptr, &resourceSet.m_ResourcePool );
			if (poolResult != VK_SUCCESS) {
				return Result<VulkanResourceSet>::Failure(
					FormatString( "vkCreateDescriptorPool failure (VkResult: {})", static_cast<int32>(poolResult) )
				);
			}

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = resourceSet.m_ResourcePool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &resourceSet.m_Layout;

			const VkResult allocResult = vkAllocateDescriptorSets( logicalDevice, &allocInfo, &resourceSet.m_ResourceSet );
			if (allocResult != VK_SUCCESS) {
				vkDestroyDescriptorPool( logicalDevice, resourceSet.m_ResourcePool, nullptr );
				vkDestroyDescriptorSetLayout( logicalDevice, resourceSet.m_Layout, nullptr );
				return Result<VulkanResourceSet>::Failure(
					FormatString( "vkAllocateDescriptorSets failure (VkResult: {})", static_cast<int32>(allocResult) )
				);
			}

			return resourceSet;

		}

	private:



	};


} // namespace lum::rhi