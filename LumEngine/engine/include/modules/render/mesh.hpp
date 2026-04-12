//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: GPU mesh resource types for static and dynamic geometry.
//
//=============================================================================//
#pragma once
#include "rhi/core/rhi_device.hpp"

namespace lum {

	/* @brief Immutable GPU-side mesh — uploaded once and never modified.
	*  Holds vertex, index and layout handles for draw calls.
	*/
	struct FStaticMeshResource {

		/* @brief Vertex buffer containing mesh geometry. */
		rhi::RBufferHandle mVbo;

		/* @brief Index buffer defining triangle topology. */
		rhi::RBufferHandle mEbo;

		/* @brief Vertex layout describing attribute bindings. */
		rhi::RVertexLayoutHandle mVao;

		/* @brief Number of indices in the index buffer. */
		uint32 mNumIndices = 0;

	};

	/* @brief Mutable GPU-side mesh — can be modified at runtime.
	*  Keeps a CPU-side copy of the mesh data and tracks whether
	*  the GPU buffers need to be re-uploaded.
	*/
	struct FDynamicMeshInstance {

		/* @brief CPU-side mesh data used to re-upload when dirty. */
		FMeshData mData;

		/* @brief Vertex buffer containing mesh geometry. */
		rhi::RBufferHandle mVbo;

		/* @brief Index buffer defining triangle topology. */
		rhi::RBufferHandle mEbo;

		/* @brief Vertex layout describing attribute bindings. */
		rhi::RVertexLayoutHandle mVao;

		/* @brief Whether the GPU buffers are out of sync with CPU data and need re-upload. */
		bool bDirty = false;

	};

} // namespace lum