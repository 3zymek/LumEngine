#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

namespace lum::rhi::vk {

	BufferHandle2 VulkanDevice::CreateBuffer( const BufferCreateInfo2& info ) noexcept {

		assert_device( );

		if (m_Buffers.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create buffer: Max buffer reached!" );
			return {};
		}

		VkBufferUsageFlags usage{};
		if (info.m_Usage.Has( BufferUsage2::Vertex )) {
			usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::Element )) {
			usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::Uniform )) {
			usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		if (info.m_Usage.Has( BufferUsage2::ShaderStorage )) {
			usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}
		if (usage == 0) {
			LUM_LOG_WARN( "Couldn't create buffer: 0 usage flags in BufferCreateInfo!" );
			return {};
		}

		VkBufferCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfo.size = info.m_BufferSize;
		vkInfo.usage = usage;

		VulkanBuffer vkBuffer{};

		if (vkCreateBuffer( m_LogicalDevice, &vkInfo, nullptr, &vkBuffer.m_VkBuffer ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create buffer: Creation failed" );
			return {};
		}

		vkGetBufferMemoryRequirements( m_LogicalDevice, vkBuffer.m_VkBuffer, &vkBuffer.m_MemoryRequirements );

		uint32 memoryTypeIndex = UINT32_MAX;
		for (uint32 i = 0; i < m_Adapter.m_MemoryProperties.memoryTypeCount; i++) {
			bool typeSupported = vkBuffer.m_MemoryRequirements.memoryTypeBits & (1 << i);
			bool hostVisible = m_Adapter.m_MemoryProperties.memoryTypes[ i ].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

			if (typeSupported && hostVisible) {
				memoryTypeIndex = i;
				break;
			}
		}

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		allocInfo.allocationSize = vkBuffer.m_MemoryRequirements.size;

		vkAllocateMemory( m_LogicalDevice, &allocInfo, nullptr, &vkBuffer.m_Memory );

		BufferHandle2 bufferHandle{};

		if (vkBindBufferMemory( m_LogicalDevice, vkBuffer.m_VkBuffer, vkBuffer.m_Memory, 0 ) != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create buffer: Binding buffer memory failed" );
			return {};
		}
		else {
			bufferHandle = m_Buffers.Append( vkBuffer );
		}
		
		// Data insertion
		if (info.m_Data != nullptr) {

			if (info.m_DataSize > info.m_BufferSize) {
				LUM_LOG_WARN( "Couldn't insert data into buffer: Data size is greater than buffer actual size" );
				return bufferHandle;
			}

			void* data = nullptr;
			VkResult res = vkMapMemory( m_LogicalDevice, vkBuffer.m_Memory, 0, info.m_DataSize, 0, &data );
			if (res != VK_SUCCESS) {
				LUM_LOG_WARN( "Couldn't map buffer memory!" );
				return bufferHandle;
			}
			memcpy( data, info.m_Data, info.m_DataSize );
			vkUnmapMemory( m_LogicalDevice, vkBuffer.m_Memory );
		}

		return bufferHandle;

	}
	void VulkanDevice::DestroyBuffer( BufferHandle2& handle ) noexcept {

		assert_device( );

		if (!m_Buffers.Contains( handle )) return;

		auto& buffer = m_Buffers[ handle ];
		vkDestroyBuffer( m_LogicalDevice, buffer.m_VkBuffer, nullptr );
		vkFreeMemory( m_LogicalDevice, buffer.m_Memory, nullptr );

		m_Buffers.Remove( handle );

	}

} // namespace lum::rhi::vk