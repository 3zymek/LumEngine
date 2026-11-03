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

#include "Rhi/Backends/Vulkan/Utils/BufferFactory.hpp"
#include "Rhi/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

namespace lum::rhi::vk {

	Result<VulkanBuffer> BufferFactory::CreateBuffer( VkDevice logicalDevice, VulkanAdapter& adapter, const BufferCreateInfo& info ) const noexcept {
		
		auto usage = ToVk( info.m_Usage );
		LUM_ASSERT( usage != 0, "Invalid usage flags in BufferCreateInfo" );

		VkBufferCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfo.size = info.m_BufferSize;
		vkInfo.usage = usage;

		VulkanBuffer buffer{};

		const VkResult result = vkCreateBuffer( logicalDevice, &vkInfo, nullptr, &buffer.m_VkBuffer );
		if (result != VK_SUCCESS) {
			return Result<VulkanBuffer>::Failure(
				FormatString( "(VkResult: {})", static_cast<int32>(result) )
			);
		}

		vkGetBufferMemoryRequirements( logicalDevice, buffer.m_VkBuffer, &buffer.m_VkMemoryRequirements );

		uint32 memoryTypeIndex = adapter.FindMemoryType(
			buffer.m_VkMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		allocInfo.allocationSize = buffer.m_VkMemoryRequirements.size;

		if (vkAllocateMemory( logicalDevice, &allocInfo, nullptr, &buffer.m_VkMemory ) != VK_SUCCESS) {
			vkDestroyBuffer( logicalDevice, buffer.m_VkBuffer, nullptr );
			return Result<VulkanBuffer>::Failure( "Buffer memory allocation failed" );
		}

		if (vkBindBufferMemory( logicalDevice, buffer.m_VkBuffer, buffer.m_VkMemory, 0 ) != VK_SUCCESS) {
			vkDestroyBuffer( logicalDevice, buffer.m_VkBuffer, nullptr );
			return Result<VulkanBuffer>::Failure( "Binding buffer memory failed" );
		}

		// Data insertion
		if (info.m_Data != nullptr) {

			LUM_ASSERT( info.m_DataSize > 0, "Invalid RHI Buffer data size" );

			void* memory = nullptr;
			VkResult res = vkMapMemory( logicalDevice, buffer.m_VkMemory, 0, info.m_DataSize, 0, &memory );
			if (res != VK_SUCCESS) {
				LUM_LOG_WARN( "Couldn't map buffer memory!" );
			}
			memcpy( memory, info.m_Data, info.m_DataSize );
			vkUnmapMemory( logicalDevice, buffer.m_VkMemory );
		}
		
		
		
		return buffer;

	}

} // namespace lum::rhi::vk
