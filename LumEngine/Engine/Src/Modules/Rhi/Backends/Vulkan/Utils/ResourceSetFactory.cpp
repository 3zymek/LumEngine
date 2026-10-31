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
#include "Rhi/Backends/Vulkan/Utils/ResourceSetFactory.hpp"

#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

namespace lum::rhi::vk {

	Result<VulkanResourceLayout> ResourceSetFactory::CreateResourceLayout( VkDevice logicalDevice, const ResourceLayoutCreateInfo& info ) {

		const usize bindingsCount = info.m_Bindings.size( );
		std::vector<VkDescriptorSetLayoutBinding> bindings( bindingsCount );

		for (uint32 i = 0; i < bindingsCount; i++) {

			auto& vkInfo = bindings[ i ];
			auto& nativeInfo = info.m_Bindings[ i ];

			auto resourceType = ToVk( nativeInfo.m_Type );
			auto stageFlags = ToVk( nativeInfo.m_ShaderStages );
			LUM_ASSERT( stageFlags, "Invalid shader stage flags" );

			vkInfo.descriptorType = resourceType;
			vkInfo.binding = nativeInfo.m_Binding;
			vkInfo.descriptorCount = nativeInfo.m_ResourceCount;
			vkInfo.stageFlags = stageFlags;
			vkInfo.pImmutableSamplers = nullptr;

		}


		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bindingsCount;
		layoutInfo.pBindings = bindings.data( );

		VulkanResourceLayout layout{};
		layout.m_Info = info;

		const VkResult layoutResult = vkCreateDescriptorSetLayout( logicalDevice, &layoutInfo, nullptr, &layout.m_VkLayout );
		if (layoutResult != VK_SUCCESS) {
			return Result<VulkanResourceLayout>::Failure(
				FormatString( "vkCreateDescriptorSetLayout failure (VkResult: {})", static_cast<int32>(layoutResult) )
			);
		}

		return layout;

	}


	Result<VulkanResourceSet> ResourceSetFactory::CreateResourceSet( VulkanDevice& device, const ResourceSetCreateInfo& info ) {

		auto& layout = device.m_ResourceLayouts[ info.m_Layout ];

		std::vector<VkDescriptorPoolSize> poolSizes{};
		for (const auto& binding : layout.m_Info.m_Bindings) {

			const auto type = ToVk( binding.m_Type );

			auto it = std::find_if( poolSizes.begin( ), poolSizes.end( ), [ type ]( const VkDescriptorPoolSize& pool ) {
				return pool.type == type;
			} );

			if (it != poolSizes.end( )) {
				it->descriptorCount += binding.m_ResourceCount;
			}
			else {
				poolSizes.push_back( { type, binding.m_ResourceCount } );
			}

		}

		VulkanResourceSet resourceSet{};
		resourceSet.m_Info = info;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = 1;
		poolInfo.poolSizeCount = poolSizes.size( );
		poolInfo.pPoolSizes = poolSizes.data( );

		const VkResult poolResult = vkCreateDescriptorPool( device.m_LogicalDevice, &poolInfo, nullptr, &resourceSet.m_VkResourcePool );
		if (poolResult != VK_SUCCESS) {
			return Result<VulkanResourceSet>::Failure(
				FormatString( "vkCreateDescriptorPool failure (VkResult: {})", static_cast<int32>(poolResult) )
			);
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = resourceSet.m_VkResourcePool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout.m_VkLayout;

		const VkResult allocResult = vkAllocateDescriptorSets( device.m_LogicalDevice, &allocInfo, &resourceSet.m_VkResourceSet );
		if (allocResult != VK_SUCCESS) {
			vkDestroyDescriptorPool( device.m_LogicalDevice, resourceSet.m_VkResourcePool, nullptr );
			return Result<VulkanResourceSet>::Failure(
				FormatString( "vkAllocateDescriptorSets failure (VkResult: {})", static_cast<int32>(allocResult) )
			);
		}

		return resourceSet;

	}

} // namespace lum::rhi::vk