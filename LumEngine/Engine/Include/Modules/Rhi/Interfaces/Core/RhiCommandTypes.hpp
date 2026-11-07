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
#include <vector>

#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Utils/FixedVector.hpp"
#include "Core/Utils/ResourceRegistry.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum::rhi::cmd {

	/* @brief Command for issuing an indexed draw call.
	* Renders geometry by referencing indices stored in an index buffer,
	* allowing vertices to be reused without duplicating vertex data.
	* Supports instanced rendering and selecting specific index and instance ranges.
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

		/* @brief Value added to each index before accessing the vertex buffer.
		* Allows the same index buffer to reference different regions of a
		* vertex buffer without modifying the stored indices.
		*/
		int32 m_VertexOffset = 0;
	};

	/* @brief Command for binding a graphics pipeline.
	* Sets the graphics pipeline used by subsequent rendering commands.
	*/
	struct LUM_API BindGraphicsPipeline : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindPipeline );

		/* @brief Handle of the graphics pipeline to bind. */
		PipelineHandle m_Handle{};
	};

	/* @brief Command for binding an index buffer.
	* Specifies the buffer containing indices used by indexed draw calls.
	*/
	struct LUM_API BindElementBuffer : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindElementBuffer );

		/* @brief Handle of the index buffer to bind. */
		BufferHandle m_BufferHandle{};

		/* @brief Byte offset from the beginning of the buffer. */
		uint32 m_StartingOffset = 0;
	};

	/* @brief Command for binding one or more vertex buffers.
	* Allows multiple vertex buffers to be bound starting at a specified binding index.
	*/
	struct LUM_API BindVertexBuffers : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindVertexBuffers );

		/* @brief Handles of the vertex buffers to bind. */
		std::span<const BufferHandle> m_BufferHandles{};

		/* @brief Index of the first vertex buffer binding. */
		uint32 m_FirstBinding = 0;
	};

	/* @brief Command for setting the viewport.
	* Defines the transformation from normalized device coordinates
	* to framebuffer coordinates and specifies the depth range.
	*/
	struct LUM_API SetViewport : Command {

		LUM_RHI_CMD_TYPE( CommandType::SetViewport );

		/* @brief Position of the viewport origin. */
		Vector2 m_Position = Vector2( 0.0f, 0.0f );

		/* @brief Width and height of the viewport. */
		Vector2u m_Size = Vector2u( 0, 0 );

		/* @brief Minimum depth value of the viewport. */
		float32 m_MinDepth = 0.0f;

		/* @brief Maximum depth value of the viewport. */
		float32 m_MaxDepth = 1.0f;
	};

	/* @brief Command for setting the scissor rectangle.
	* Restricts rendering operations to a specified framebuffer region.
	* Fragments outside the scissor rectangle are discarded.
	*/
	struct LUM_API SetScissors : Command {

		LUM_RHI_CMD_TYPE( CommandType::SetScissors );

		/* @brief Offset of the scissor rectangle from the framebuffer origin. */
		Vector2i m_Offset = Vector2i( 0, 0 );

		/* @brief Width and height of the scissor rectangle. */
		Vector2u m_Extent = Vector2u( 0, 0 );
	};

	/* @brief Describes a buffer resource update within a resource set.
	* Specifies the buffer and the range of data associated with a descriptor binding.
	*/
	struct LUM_API UpdateBufferInfo {

		/* @brief Handle of the buffer to bind. */
		BufferHandle m_BufferHandle{};

		/* @brief Byte offset into the buffer. */
		uint64 m_Offset = 0;

		/* @brief Size of the buffer range in bytes. */
		uint64 m_Range = 0;

		/* @brief Array element index within the descriptor binding. */
		uint32 m_ArrayIndex = 0;

		/* @brief Descriptor binding index. */
		uint32 m_Binding = 0;

	};

	/* @brief Describes an image resource update within a resource set.
	* Specifies the image and sampler associated with a descriptor binding.
	*/
	struct LUM_API UpdateImageInfo {

		/* @brief Handle of the sampler to associate with the image. */
		SamplerHandle m_SamplerHandle{};

		/* @brief Handle of the image to bind. */
		ImageHandle m_ImageHandle{};

		/* @brief Array element index within the descriptor binding. */
		uint32 m_ArrayIndex = 0;

		/* @brief Descriptor binding index. */
		uint32 m_Binding = 0;

	};

	/* @brief Command for updating the contents of a resource set.
	* Provides buffer and image resources that will be associated with
	* their respective descriptor bindings.
	*/
	struct LUM_API UpdateResourceSet : Command {

		LUM_RHI_CMD_TYPE( CommandType::UpdateResourceSet );

		/* @brief Handle of the resource set to update. */
		ResourceSetHandle m_SetHandle{};

		/* @brief Image resources to associate with the resource set. (max is 4) */
		FixedVector<UpdateImageInfo, 4> m_ImageInfos{};

		/* @brief Buffer resources to associate with the resource set. (max is 4) */
		FixedVector<UpdateBufferInfo, 4> m_BufferInfos{};
	};

	/* @brief Command for binding a resource set.
	* Makes the resources stored in the specified resource set available
	* to subsequent shader execution.
	*/
	struct LUM_API BindResourceSet : Command {

		LUM_RHI_CMD_TYPE( CommandType::BindResourceSet );

		/* @brief Handle of the resource set to bind. */
		ResourceSetHandle m_SetHandle{};
	};

	/* @brief Command for transferring image data.
	* Uploads image data to a GPU image resource.
	* Scheduled during the pre-render stage to prepare image resources
	* before rendering commands are executed.
	*/
	struct LUM_API TransferImage : Command {

		LUM_RHI_CMD_TYPE( CommandType::TransferImage );
		LUM_RHI_CMD_PRERENDER( );

		/* @brief Image data to upload. */
		ResourceImageHandle m_DataHandle{};

		/* @brief Handle of the destination image resource. */
		ImageHandle m_Image{};

	};



	/* @brief Command for updating shader push constants.
	* Provides a small block of data directly to shader stages
	* without requiring a buffer resource or descriptor binding.
	*/
	struct LUM_API PushConstants : Command {

		LUM_RHI_CMD_TYPE( CommandType::PushConstants );

		/* @brief Pointer to the data to upload. */
		void* m_Data = nullptr;

		/* @brief Size of the data in bytes. */
		usize m_Size = 0;

		/* @brief Offset into the push constant storage in bytes. */
		usize m_Offset = 0;

		/* @brief Shader stages that can access the specified push constants. */
		ShaderStageFlags m_ShaderStageFlags{};
	};

} // namespace lum::rhi::cmd