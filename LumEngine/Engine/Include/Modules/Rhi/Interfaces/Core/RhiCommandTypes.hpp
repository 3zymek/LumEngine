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

#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"

namespace lum::rhi::cmd {


    /*
    * @brief Represents a command for issuing an indexed draw call.
    *  
    * DrawIndexed renders geometry by referencing indices stored in an index
    * buffer, allowing vertices to be reused without duplicating vertex data.
    * The command supports instanced rendering and allows selecting a subset
    * of indices and instances to draw.
    */
    struct DrawIndexed : Command {

        LUM_RHI_CMD_TYPE( CommandType::DrawIndexed );

        /* @brief Number of indices to read from the index buffer. */
        uint32 m_IndexCount = 0;

        /* @brief Number of instances to render. */
        uint32 m_InstanceCount = 0;

        /* @brief Index of the first index to read from the index buffer. */
        uint32 m_FirstIndex = 0;

        /* @brief Index of the first instance to render. */
        uint32 m_FirstInstance = 0;

        /*
         * @brief Value added to each index before accessing the vertex buffer.
         *
         * Allows the same index buffer to reference different regions of a
         * vertex buffer without modifying the stored indices.
         */
        int32 m_VertexOffset = 0;
    };

	struct BindGraphicsPipeline : Command {
		LUM_RHI_CMD_TYPE( CommandType::BindPipeline );
		PipelineHandle m_Handle{};
	};

	struct BindElementBuffer : Command {
		LUM_RHI_CMD_TYPE( CommandType::BindElementBuffer );
        BufferHandle m_Handle{};
        uint32 m_StartingOffset = 0;

	};

    struct BindVertexBuffers : Command {
        LUM_RHI_CMD_TYPE( CommandType::BindVertexBuffers );
        std::span<const BufferHandle> m_Handles{};
        uint32 m_FirstBinding = 0;
    };

} // namespace lum::rhi