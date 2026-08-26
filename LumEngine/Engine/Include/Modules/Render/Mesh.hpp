//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: GPU mesh resource types for static and dynamic geometry.
//
//=============================================================================//
#pragma once

#include "Rhi/Core/RhiDevice.hpp"

namespace lum {

	/* @brief Immutable GPU-side mesh — uploaded once and never modified.
	*  Holds vertex, index and layout handles for draw calls.
	*/
	struct StaticMeshResource {

		/* @brief Vertex buffer containing mesh geometry. */
		rhi::BufferHandle m_Vbo;

		/* @brief Index buffer defining triangle topology. */
		rhi::BufferHandle m_Ebo;

		/* @brief Vertex layout describing attribute bindings. */
		rhi::VertexLayoutHandle m_Vao;

		/* @brief Number of indices in the index buffer. */
		uint32 m_NumIndices = 0;

	};

	/* @brief Mutable GPU-side mesh — can be modified at runtime.
	*  Keeps a CPU-side copy of the mesh data and tracks whether
	*  the GPU buffers need to be re-uploaded.
	*/
	struct DynamicMeshInstance {

		/* @brief CPU-side mesh data used to re-upload when dirty. */
		MeshGeometry m_Data;

		/* @brief Vertex buffer containing mesh geometry. */
		rhi::BufferHandle m_Vbo;

		/* @brief Index buffer defining triangle topology. */
		rhi::BufferHandle m_Ebo;

		/* @brief Vertex layout describing attribute bindings. */
		rhi::VertexLayoutHandle m_Vao;

		/* @brief Whether the GPU buffers are out of sync with CPU data and need re-upload. */
		bool m_Dirty = false;

	};

} // namespace lum