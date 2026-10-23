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
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"

#include "Core/Utils/Assert.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"

namespace lum::rhi::vk {

	void VulkanDevice::bind_commands_implementations( ) noexcept {

		m_CommandProcessor.RegisterCommandCall<cmd::DrawIndexed>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::DrawIndexed& cmd ) {

				LUM_ASSERT( cmd.m_IndexCount > 0 && cmd.m_InstanceCount > 0, "DrawIndexed called with 0 indices or instances!" );

				vkCmdDrawIndexed( vkCmdBuffer, cmd.m_IndexCount, cmd.m_InstanceCount, cmd.m_FirstIndex, cmd.m_VertexOffset, cmd.m_FirstInstance );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindGraphicsPipeline>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindGraphicsPipeline& cmd ) {

				LUM_ASSERT( m_Pipelines.Contains( cmd.m_Handle ), "Called BindPipeline on Pipeline that doesn't exist!" );

				auto& pip = m_Pipelines[ cmd.m_Handle ];

				vkCmdBindPipeline( vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pip.m_Pipeline );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindElementBuffer>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindElementBuffer& cmd ) {

				LUM_ASSERT( m_Buffers.Contains( cmd.m_Handle ), "Called BindElementBuffer on Buffer that doesn't exist!" );

				auto& buffer = m_Buffers[ cmd.m_Handle ];

				vkCmdBindIndexBuffer( vkCmdBuffer, buffer.m_VkBuffer, cmd.m_StartingOffset, VK_INDEX_TYPE_UINT32 );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::BindVertexBuffers>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindVertexBuffers& cmd ) {

				const usize count = cmd.m_Handles.size( );
				if (count == 0 || count > k_MaxVertexBufferBindCount) {
					LUM_LOG_WARN( "Vertex buffer bind overflow, increase limit (Current: {}) or refactor your code.", k_MaxVertexBufferBindCount );
					return;
				}

				VkBuffer buffers[ k_MaxVertexBufferBindCount ]{};

				for (uint32 i = 0; i < cmd.m_Handles.size( ); i++) {

					const auto& handle = cmd.m_Handles[ i ];
					LUM_ASSERT( m_Buffers.Contains( handle ), "Called BindVertexBuffer on Buffer that doesn't exist!" );
					buffers[ i ] = m_Buffers[ handle ].m_VkBuffer;

				}

				VkDeviceSize offset = 0;

				vkCmdBindVertexBuffers( vkCmdBuffer, cmd.m_FirstBinding, count, buffers, &offset );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::SetViewport>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::SetViewport& cmd ) {

				auto size = cmd.m_Size;
				
				if (size.m_X == 0 && size.m_Y == 0) {
					size = { m_SwapchainExtent.width, m_SwapchainExtent.height };
				}
				


				VkViewport viewport{};
				viewport.x = cmd.m_Position.m_X;
				viewport.y = cmd.m_Position.m_Y;
				viewport.width = size.m_X;
				viewport.height = size.m_Y;
				viewport.minDepth = cmd.m_MinDepth;
				viewport.maxDepth = cmd.m_MaxDepth;

				vkCmdSetViewport( vkCmdBuffer, 0, 1, &viewport );

			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::SetScissors>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::SetScissors& cmd ) {

				VkRect2D scissor{};
				scissor.offset = { cmd.m_Offset.m_X, cmd.m_Offset.m_Y };

				if (cmd.m_Extent.m_X == 0 && cmd.m_Extent.m_Y == 0) {
					scissor.extent = m_SwapchainExtent;
				}
				else {
					scissor.extent = { cmd.m_Extent.m_X, cmd.m_Extent.m_Y };
				}

				vkCmdSetScissor( vkCmdBuffer, 0, 1, &scissor );

			}
		);


		m_CommandProcessor.RegisterCommandCall<cmd::UpdateResourceSet>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::UpdateResourceSet& cmd ) {

				LUM_ASSERT( m_ResourceSets.Contains( cmd.m_SetHandle ), "Called UpdateResourceBuffer on ResourceSet that doesn't exist!" );

				auto& set = m_ResourceSets[ cmd.m_SetHandle ];

				VkWriteDescriptorSet info{};
				info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				info.dstSet = set.m_ResourceSet;
				info.dstBinding = cmd.m_Binding;
				info.dstArrayElement = cmd.m_ArrayIndex;
				info.descriptorCount = 1;
				info.descriptorType;
				
				VkDescriptorBufferInfo bufferInfo{};
				if (m_Buffers.Contains( cmd.m_Buffer.m_BufferHandle )) {
					bufferInfo.buffer = m_Buffers[ cmd.m_Buffer.m_BufferHandle ].m_VkBuffer;
					bufferInfo.offset = cmd.m_Buffer.m_Offset;
					bufferInfo.range = cmd.m_Buffer.m_Range == 0 ? VK_WHOLE_SIZE : cmd.m_Buffer.m_Range;
					info.pBufferInfo = &bufferInfo;
				}
				VkDescriptorImageInfo imageInfo{};
				if (m_Images.Contains( cmd.m_Image.m_ImageHandle )) {
					imageInfo.imageView = m_Images[ cmd.m_Image.m_ImageHandle ].m_View;

				}

				
				
			}
		);

		m_CommandProcessor.RegisterCommandCall<cmd::SetScissors>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::SetScissors& cmd ) {

				VkRect2D scissor{};
				scissor.offset = { cmd.m_Offset.m_X, cmd.m_Offset.m_Y };

				if (cmd.m_Extent.m_X == 0 && cmd.m_Extent.m_Y == 0) {
					scissor.extent = m_SwapchainExtent;
				}
				else {
					scissor.extent = { cmd.m_Extent.m_X, cmd.m_Extent.m_Y };
				}

				vkCmdSetScissor( vkCmdBuffer, 0, 1, &scissor );

			}
		);


	}

} // namespace lum::rhi::vk