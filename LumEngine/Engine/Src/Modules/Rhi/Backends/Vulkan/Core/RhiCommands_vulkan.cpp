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

#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

#include "Core/Utils/Assert.hpp"
#include "Rhi/Backends/Vulkan/RhiEnumCast_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"

namespace lum::rhi::vk {

	void VulkanDevice::bind_commands_implementations( ) noexcept {

		m_CommandProcessor.RegisterCommandCall<cmd::DrawIndexed>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::DrawIndexed& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE ||
					cmd.m_IndexCount == 0 ||
					cmd.m_InstanceCount == 0) {
					LUM_LOG_WARN( "DrawIndexed: Invalid command parameters." );
					return;
				}

				vkCmdDrawIndexed(
					vkCmdBuffer,
					cmd.m_IndexCount,
					cmd.m_InstanceCount,
					cmd.m_FirstIndex,
					cmd.m_VertexOffset,
					cmd.m_FirstInstance
				);

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindGraphicsPipeline>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindGraphicsPipeline& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE ||
					!m_Pipelines.Contains( cmd.m_Handle )) {
					LUM_LOG_WARN( "BindGraphicsPipeline: Invalid command buffer or pipeline handle." );
					return;
				}

				auto& pip = m_Pipelines[ cmd.m_Handle ];

				if (pip.m_VkPipeline == VK_NULL_HANDLE ||
					pip.m_VkLayout == VK_NULL_HANDLE) {
					LUM_LOG_WARN( "BindGraphicsPipeline: Invalid Vulkan pipeline or pipeline layout." );
					return;
				}

				vkCmdBindPipeline(
					vkCmdBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					pip.m_VkPipeline
				);

				m_BoundPipelineLayout = pip.m_VkLayout;

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindElementBuffer>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindElementBuffer& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE ||
					!m_Buffers.Contains( cmd.m_BufferHandle )) {
					LUM_LOG_WARN( "BindElementBuffer: Invalid command buffer or buffer handle." );
					return;
				}

				auto& buffer = m_Buffers[ cmd.m_BufferHandle ];

				if (buffer.m_VkBuffer == VK_NULL_HANDLE) {
					LUM_LOG_WARN( "BindElementBuffer: Invalid Vulkan buffer." );
					return;
				}

				vkCmdBindIndexBuffer(
					vkCmdBuffer,
					buffer.m_VkBuffer,
					cmd.m_StartingOffset,
					VK_INDEX_TYPE_UINT32
				);

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindVertexBuffers>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindVertexBuffers& cmd ) {

				const usize count = cmd.m_BufferHandles.size( );

				if (vkCmdBuffer == VK_NULL_HANDLE ||
					count == 0 ||
					count > k_MaxVertexBufferBindCount ||
					cmd.m_FirstBinding > std::numeric_limits<uint32>::max( ) - count) {
					LUM_LOG_WARN( "BindVertexBuffers: Invalid command parameters." );
					return;
				}

				VkBuffer buffers[ k_MaxVertexBufferBindCount ]{};

				for (usize i = 0; i < count; ++i) {

					const auto& handle = cmd.m_BufferHandles[ i ];

					if (!m_Buffers.Contains( handle )) {
						LUM_LOG_WARN( "BindVertexBuffers: Buffer handle does not exist." );
						return;
					}

					const auto& buffer = m_Buffers[ handle ];

					if (buffer.m_VkBuffer == VK_NULL_HANDLE) {
						LUM_LOG_WARN( "BindVertexBuffers: Invalid Vulkan buffer." );
						return;
					}

					buffers[ i ] = buffer.m_VkBuffer;

				}

				const VkDeviceSize offset = 0;

				vkCmdBindVertexBuffers(
					vkCmdBuffer,
					cmd.m_FirstBinding,
					static_cast<uint32>(count),
					buffers,
					&offset
				);

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::SetViewport>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::SetViewport& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE) {
					LUM_LOG_WARN( "SetViewport: Invalid command buffer." );
					return;
				}

				auto size = cmd.m_Size;

				if (size.m_X == 0 && size.m_Y == 0) {
					size = {
						m_SwapchainExtent.width,
						m_SwapchainExtent.height
					};
				}

				if (size.m_X == 0 ||
					size.m_Y == 0 ||
					cmd.m_MinDepth < 0.0f ||
					cmd.m_MaxDepth > 1.0f ||
					cmd.m_MinDepth > cmd.m_MaxDepth) {

					LUM_LOG_WARN( "SetViewport: Invalid viewport parameters." );
					return;

				}

				VkViewport viewport{};
				viewport.x = static_cast<float32>(cmd.m_Position.m_X);
				viewport.y = static_cast<float32>(cmd.m_Position.m_Y);
				viewport.width = static_cast<float32>(size.m_X);
				viewport.height = static_cast<float32>(size.m_Y);
				viewport.minDepth = cmd.m_MinDepth;
				viewport.maxDepth = cmd.m_MaxDepth;

				vkCmdSetViewport( vkCmdBuffer, 0, 1, &viewport );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::SetScissors>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::SetScissors& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE) {
					LUM_LOG_WARN( "SetScissors: Invalid command buffer." );
					return;
				}

				VkRect2D scissor{};
				scissor.offset = {
					cmd.m_Offset.m_X,
					cmd.m_Offset.m_Y
				};

				if (cmd.m_Extent.m_X == 0 && cmd.m_Extent.m_Y == 0) {
					scissor.extent = m_SwapchainExtent;
				}
				else if (cmd.m_Extent.m_X == 0 || cmd.m_Extent.m_Y == 0) {
					LUM_LOG_WARN( "SetScissors: Invalid scissor extent." );
					return;
				}
				else {
					scissor.extent = {
						cmd.m_Extent.m_X,
						cmd.m_Extent.m_Y
					};
				}

				if (scissor.offset.x < 0 ||
					scissor.offset.y < 0 ||
					static_cast<uint64>( scissor.offset.x ) + scissor.extent.width > m_SwapchainExtent.width ||
					static_cast<uint64>( scissor.offset.y ) + scissor.extent.height > m_SwapchainExtent.height) {
					LUM_LOG_WARN( "SetScissors: Scissor rectangle exceeds swapchain extent." );
					return;
				}

				vkCmdSetScissor( vkCmdBuffer, 0, 1, &scissor );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::UpdateResourceSet>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::UpdateResourceSet& cmd ) {

				LUM_ASSERT( m_LogicalDevice != VK_NULL_HANDLE, "UpdateResourceSet: Invalid logical device!" );
				LUM_ASSERT( m_ResourceSets.Contains( cmd.m_SetHandle ), "Called UpdateResourceBuffer on ResourceSet that doesn't exist!" );

				auto& set = m_ResourceSets[ cmd.m_SetHandle ];
				auto& layout = m_ResourceLayouts[ set.m_Info.m_Layout ];

				std::vector<VkWriteDescriptorSet> writeSets{};
				std::vector<VkDescriptorBufferInfo> bufferInfos{};
				std::vector<VkDescriptorImageInfo> imageInfos{};

				const auto writeCount = cmd.m_BufferInfos.size( ) + cmd.m_ImageInfos.size( );

				writeSets.reserve( writeCount );
				bufferInfos.reserve( cmd.m_BufferInfos.size( ) );
				imageInfos.reserve( cmd.m_ImageInfos.size( ) );

				for (const auto& bufferInfo : cmd.m_BufferInfos) {

					LUM_ASSERT( m_Buffers.Contains( bufferInfo.m_BufferHandle ), "UpdateResourceSet: Updated buffer doesn't exist!" );

					auto& buffer = m_Buffers[ bufferInfo.m_BufferHandle ];

					const auto& bindings = layout.m_Info.m_Bindings;
					auto it = std::find_if( bindings.begin( ), bindings.end( ), [ &bufferInfo ]( const auto& b ) {
						return b.m_Binding == bufferInfo.m_Binding;
					} );

					LUM_ASSERT( it != bindings.end( ), "UpdateResourceSet: Buffer binding {} not found in ResourceSet!", bufferInfo.m_Binding );
					LUM_ASSERT( it->m_Type == ResourceType::UniformBuffer || it->m_Type == ResourceType::StorageBuffer, "UpdateResourceSet: Buffer resource type does not match binding {}!", bufferInfo.m_Binding );
					LUM_ASSERT( bufferInfo.m_ArrayIndex < it->m_ResourceCount, "UpdateResourceSet: Array index of Buffer {} out of bounds for binding {} (Max: {})!", bufferInfo.m_ArrayIndex, bufferInfo.m_Binding, it->m_ResourceCount );
					LUM_ASSERT( bufferInfo.m_Offset <= buffer.m_Info.m_Size, "UpdateResourceSet: Buffer offset exceeds buffer size!" );

					const VkDeviceSize range = bufferInfo.m_Range == 0 ? VK_WHOLE_SIZE : bufferInfo.m_Range;

					LUM_ASSERT( range == VK_WHOLE_SIZE || range <= buffer.m_Info.m_Size - bufferInfo.m_Offset, "UpdateResourceSet: Buffer range exceeds buffer size!" );

					VkDescriptorBufferInfo vkBufferInfo{};
					vkBufferInfo.buffer = buffer.m_VkBuffer;
					vkBufferInfo.offset = bufferInfo.m_Offset;
					vkBufferInfo.range = range;

					bufferInfos.push_back( vkBufferInfo );

					VkWriteDescriptorSet vkInfo{};
					vkInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vkInfo.dstSet = set.m_VkResourceSet;
					vkInfo.dstBinding = bufferInfo.m_Binding;
					vkInfo.dstArrayElement = bufferInfo.m_ArrayIndex;
					vkInfo.descriptorCount = 1;
					vkInfo.descriptorType = ToVk( it->m_Type );
					vkInfo.pBufferInfo = &bufferInfos.back( );

					writeSets.push_back( vkInfo );
				}

				for (const auto& imageInfo : cmd.m_ImageInfos) {

					LUM_ASSERT( m_Images.Contains( imageInfo.m_ImageHandle ), "UpdateResourceSet: Updated image doesn't exist!" );

					auto& image = m_Images[ imageInfo.m_ImageHandle ];

					const auto& bindings = layout.m_Info.m_Bindings;
					auto it = std::find_if( bindings.begin( ), bindings.end( ), [ &imageInfo ]( const auto& b ) {
						return b.m_Binding == imageInfo.m_Binding;
					} );

					LUM_ASSERT( it != bindings.end( ), "UpdateResourceSet: Image binding {} not found in ResourceSet!", imageInfo.m_Binding );
					LUM_ASSERT( it->m_Type == ResourceType::CombinedImageSampler || it->m_Type == ResourceType::SampledImage || it->m_Type == ResourceType::StorageImage, "UpdateResourceSet: Image resource type does not match binding {}!", imageInfo.m_Binding );
					LUM_ASSERT( imageInfo.m_ArrayIndex < it->m_ResourceCount, "UpdateResourceSet: Array index of Image {} out of bounds for binding {} (Max: {})!", imageInfo.m_ArrayIndex, imageInfo.m_Binding, it->m_ResourceCount );

					VkDescriptorImageInfo vkImageInfo{};
					vkImageInfo.imageView = image.m_VkView;
					vkImageInfo.imageLayout = ToVk( image.m_Info.m_CurrentLayout );

					if (it->m_Type == ResourceType::CombinedImageSampler) {
						LUM_ASSERT( m_Samplers.Contains( imageInfo.m_SamplerHandle ), "UpdateResourceSet: Missing sampler for combined image sampler descriptor!" );
						vkImageInfo.sampler = m_Samplers[ imageInfo.m_SamplerHandle ].m_VkSampler;
					}
					else if (it->m_Type == ResourceType::SampledImage) {
						LUM_ASSERT( image.m_Info.m_CurrentLayout == ImageLayout::ShaderReadOnly, "UpdateResourceSet: Sampled image is not in shader-read-only layout!" );
					}
					else if (it->m_Type == ResourceType::StorageImage) {
						LUM_ASSERT( image.m_Info.m_CurrentLayout == ImageLayout::General, "UpdateResourceSet: Storage image is not in general layout!" );
					}

					imageInfos.push_back( vkImageInfo );

					VkWriteDescriptorSet vkInfo{};
					vkInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vkInfo.dstSet = set.m_VkResourceSet;
					vkInfo.dstBinding = imageInfo.m_Binding;
					vkInfo.dstArrayElement = imageInfo.m_ArrayIndex;
					vkInfo.descriptorCount = 1;
					vkInfo.descriptorType = ToVk( it->m_Type );
					vkInfo.pImageInfo = &imageInfos.back( );

					writeSets.push_back( vkInfo );
				}

				if (!writeSets.empty( ))
					vkUpdateDescriptorSets( m_LogicalDevice, static_cast<uint32>(writeSets.size( )), writeSets.data( ), 0, nullptr );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindResourceSet>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindResourceSet& cmd ) {

				if (vkCmdBuffer == VK_NULL_HANDLE ||
					m_BoundPipelineLayout == VK_NULL_HANDLE ||
					!m_ResourceSets.Contains( cmd.m_SetHandle )) {
					LUM_LOG_WARN( "BindResourceSet: Invalid command buffer, pipeline layout or resource set." );
					return;
				}

				auto& set = m_ResourceSets[ cmd.m_SetHandle ];

				if (set.m_VkResourceSet == VK_NULL_HANDLE) {
					LUM_LOG_WARN( "BindResourceSet: Invalid Vulkan descriptor set." );
					return;
				}

				vkCmdBindDescriptorSets(
					vkCmdBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					m_BoundPipelineLayout,
					0,
					1,
					&set.m_VkResourceSet,
					0,
					nullptr
				);

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::TransferImage>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::TransferImage& cmd ) {

				if (cmd.m_Data.m_Width == 0 || cmd.m_Data.m_Height == 0 || cmd.m_Data.m_Pixels.empty( )) {
					LUM_LOG_WARN( "TransferImage: Invalid image dimensions or pixel data." );
					return;
				}

				if (!m_Images.Contains( cmd.m_Image )) {
					LUM_LOG_WARN( "TransferImage: Image handle does not exist." );
					return;
				}

				auto& image = m_Images[ cmd.m_Image ];

				if (cmd.m_Data.m_Width != image.m_Info.m_Extent.m_Width ||
					cmd.m_Data.m_Height != image.m_Info.m_Extent.m_Height) {
					LUM_LOG_WARN( "TransferImage: Pixel dimensions do not match image dimensions." );
					return;
				}

				const VkDeviceSize dataSize =
					static_cast<VkDeviceSize>(cmd.m_Data.m_Pixels.size( ));

				if (dataSize == 0) {
					LUM_LOG_WARN( "TransferImage: Invalid staging buffer range." );
					return;
				}

				VulkanBuffer stagingBuffer = m_StagingBuffers[ m_CurrentFrame ];
				BufferHandle fallbackBufferHandle{};

				if (stagingBuffer.m_Info.m_Size < dataSize) {
					BufferCreateInfo bufferInfo{};
					bufferInfo.m_BufferSize = dataSize;
					bufferInfo.m_DataSize = dataSize;
					bufferInfo.m_Data = cmd.m_Data.m_Pixels.data( );
					bufferInfo.m_Usage = BufferUsage::TransferSource;

					fallbackBufferHandle = CreateBuffer( bufferInfo );
					LUM_ASSERT( m_Buffers.Contains( fallbackBufferHandle ), "TransferImage: Couldn't create fallback staging buffer" );
					stagingBuffer = m_Buffers[ fallbackBufferHandle ];
				}
				else {

					void* mapped = nullptr;

					const VkResult mapResult = vkMapMemory(
						m_LogicalDevice,
						stagingBuffer.m_VkMemory,
						0,
						dataSize,
						0,
						&mapped
					);

					if (mapResult != VK_SUCCESS || mapped == nullptr) {
						LUM_LOG_WARN( "TransferImage: Failed to map staging memory. VkResult: {}", static_cast<int>(mapResult) );
						return;
					}

					std::memcpy(
						mapped,
						cmd.m_Data.m_Pixels.data( ),
						static_cast<usize>(dataSize)
					);

					vkUnmapMemory(
						m_LogicalDevice,
						stagingBuffer.m_VkMemory
					);

				}

				detail::TransitionImageInfo info{};
				info.m_Image = image;
				info.m_NewLayout = ImageLayout::TransferDestination;
				info.m_NewStage = VK_PIPELINE_STAGE_2_COPY_BIT;
				info.m_NewAccess = VK_ACCESS_2_TRANSFER_WRITE_BIT;
				info.m_VkCommandBuffer = vkCmdBuffer;

				transition_image_layout( info );

				VkBufferImageCopy region{};
				region.bufferOffset = 0;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;

				region.imageSubresource.aspectMask =
					GetAspectMask( image.m_Info.m_Format );
				region.imageSubresource.mipLevel = 0;
				region.imageSubresource.baseArrayLayer = 0;
				region.imageSubresource.layerCount = 1;

				region.imageOffset = { 0, 0, 0 };
				region.imageExtent = {
					cmd.m_Data.m_Width,
					cmd.m_Data.m_Height,
					1
				};

				vkCmdCopyBufferToImage(
					vkCmdBuffer,
					stagingBuffer.m_VkBuffer,
					image.m_VkImage,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&region
				);

				detail::TransitionImageInfo readInfo{};
				readInfo.m_Image = image;
				readInfo.m_NewLayout = ImageLayout::ShaderReadOnly;
				readInfo.m_NewStage = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
				readInfo.m_NewAccess = VK_ACCESS_2_SHADER_READ_BIT;
				readInfo.m_VkCommandBuffer = vkCmdBuffer;

				transition_image_layout( readInfo );

				m_TemporaryStagingBuffers[ m_CurrentFrame ].push( fallbackBufferHandle );

			}
		);

	}

} // namespace lum::rhi::vk
