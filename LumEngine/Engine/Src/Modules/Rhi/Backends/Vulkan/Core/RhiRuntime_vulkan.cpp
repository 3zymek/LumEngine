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
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"

namespace lum::rhi::vk {

	void VulkanDevice::bind_commands_implementations( ) noexcept {

		m_CommandInterpreter.RegisterTypedCall<cmd::DrawIndexed>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::DrawIndexed& cmd ) {

				LUM_ASSERT( cmd.m_IndexCount > 0 && cmd.m_InstanceCount > 0, "DrawIndexed called with 0 indices or instances!" );

				vkCmdDrawIndexed( vkCmdBuffer, cmd.m_IndexCount, cmd.m_InstanceCount, cmd.m_FirstIndex, cmd.m_VertexOffset, cmd.m_FirstInstance );

			}
		);

		m_CommandInterpreter.RegisterTypedCall<cmd::BindGraphicsPipeline>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindGraphicsPipeline& cmd ) {

				LUM_ASSERT( m_Pipelines.Contains( cmd.m_Handle ), "Called BindPipeline on pipeline that doesn't exist!" );

				auto& pip = m_Pipelines[ cmd.m_Handle ];

				vkCmdBindPipeline( vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pip.m_Pipeline );

			}
		);

		m_CommandInterpreter.RegisterTypedCall<cmd::BindElementBuffer>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindElementBuffer& cmd ) {

				LUM_ASSERT( m_Buffers.Contains( cmd.m_Handle ), "Called BindElementBuffer on buffer that doesn't exist!" );

				auto& buffer = m_Buffers[ cmd.m_Handle ];

				vkCmdBindIndexBuffer( vkCmdBuffer, buffer.m_VkBuffer, cmd.m_StartingOffset, VK_INDEX_TYPE_UINT32 );

			}
		);

		m_CommandInterpreter.RegisterTypedCall<cmd::BindVertexBuffers>(
			[ this ]( VkCommandBuffer vkCmdBuffer, const cmd::BindVertexBuffers& cmd ) {

				const usize count = cmd.m_Handles.size( );
				if (count == 0 || count > k_MaxVertexBufferBindCount) {
					LUM_LOG_WARN( "Vertex buffer bind overflow, increase limit (Current: {}) or refactor your code.", k_MaxVertexBufferBindCount );
					return;
				}

				VkBuffer buffers[ k_MaxVertexBufferBindCount ]{};

				for (uint32 i = 0; i < cmd.m_Handles.size( ); i++) {

					const auto& handle = cmd.m_Handles[ i ];
					LUM_ASSERT( m_Buffers.Contains( handle ), "Called BindVertexBuffer on buffer that doesn't exist!" );
					buffers[ i ] = m_Buffers[ handle ].m_VkBuffer;

				}

				VkDeviceSize offset = 0;

				vkCmdBindVertexBuffers( vkCmdBuffer, cmd.m_FirstBinding, count, buffers, &offset );

			}
		);


	}

} // namespace lum::rhi::vk