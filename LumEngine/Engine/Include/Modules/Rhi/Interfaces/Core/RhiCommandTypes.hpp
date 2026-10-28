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

#include <span>

#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::cmd {

	/*
	* @brief Represents a command for issuing an indexed draw call.
	*
	* DrawIndexed renders geometry by referencing indices stored in an index
	* buffer, allowing vertices to be reused without duplicating vertex data.
	* The command supports instanced rendering and allows selecting a subset
	* of indices and instances to draw.
	*/
	struct LUM_API DrawIndexed : Command {

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

	struct LUM_API BindGraphicsPipeline : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindPipeline );

		PipelineHandle m_Handle{};

	};

	struct LUM_API BindElementBuffer : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindElementBuffer );

		BufferHandle m_BufferHandle{};
		uint32 m_StartingOffset = 0;

	};

	struct LUM_API BindVertexBuffers : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindVertexBuffers );

		std::span<const BufferHandle> m_BufferHandles{};
		uint32 m_FirstBinding = 0;

	};

	struct LUM_API SetViewport : Command {

		LUM_RHI_CMD_TYPE( CommandType::SetViewport );

		Vector2 m_Position = Vector2( 0.0f, 0.0f );
		Vector2u m_Size = Vector2u( 0, 0 );

		float32 m_MinDepth = 0.0f;
		float32 m_MaxDepth = 1.0f;

	};

	struct LUM_API SetScissors : Command {

		LUM_RHI_CMD_TYPE( CommandType::SetScissors );

		Vector2i m_Offset = Vector2i( 0, 0 );
		Vector2u m_Extent = Vector2u( 0, 0 );

	};

	struct LUM_API UpdateResourceSet : Command {

		LUM_RHI_CMD_TYPE( CommandType::UpdateResourceSet );

		ResourceSetHandle m_SetHandle{};

		uint32 m_Binding = 0;
		uint32 m_ArrayIndex = 0;

		struct {
			BufferHandle m_BufferHandle{};
			uint64 m_Offset = 0;
			uint64 m_Range = 0;
		} m_Buffer;

		struct {
			SamplerHandle m_SamplerHandle{};
			ImageHandle m_ImageHandle{};
		} m_Image;

	};

	struct LUM_API BindResourceSet : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindResourceSet );

		ResourceSetHandle m_SetHandle{};

	};

	struct LUM_API TransferImage : Command {

		LUM_RHI_CMD_TYPE( CommandType::TransferImage );
		LUM_RHI_CMD_PRERENDER( );

		ImageData m_Data{};
		ImageHandle m_Image{};
		usize m_StagingOffset = 0;

	};

} // namespace lum::rhi