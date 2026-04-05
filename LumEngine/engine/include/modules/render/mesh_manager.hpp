//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Loads and manages static mesh resources using RHI.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "render/render_common.hpp"

#include "render/mesh.hpp"

namespace lum {

	namespace detail {

		enum class MeshType : byte {
			Static,
			Dynamic
		};

		/* @brief Stores raw GPU buffer handles for a mesh (VBO, EBO, VAO). */
		struct FRenderResources {
			rhi::RBufferHandle			mVbo;
			rhi::RBufferHandle			mEbo;
			rhi::RVertexLayoutHandle	mVao;
		};

	} // namespace lum::detail

	/* @brief Loads and manages static mesh resources on the GPU.
	*
	* Handles mesh loading via Assimp, GPU upload, and handle-based access.
	* Meshes are cached by path hash to avoid duplicate uploads.
	* Provides built-in default and error meshes as fallbacks.
	*/
	class MMeshManager {
	public:

		MMeshManager() {}

		void Initialize( render::FRendererContext* ctx );

		/* @brief Returns the static mesh resource for the given handle.
		* @param handle Handle to the static mesh.
		* @return Reference to the mesh resource containing VAO and index count.
		*/
		const FStaticMeshResource& GetStatic( StaticMeshHandle handle );

		/* @brief Loads a static mesh from disk and uploads it to the GPU.
		* Returns a cached handle if the mesh was already loaded.
		* @param path Path to the mesh file (relative to root).
		* @param root Root directory identifier.
		* @return Handle to the uploaded static mesh.
		*/
		StaticMeshHandle CreateStatic( StringView path, RootID root = RootID::External );

		/* @brief Loads a dynamic mesh instance from disk.
		* @param path Path to the mesh file.
		* @param root Root directory identifier.
		* @return Dynamic mesh instance.
		*/
		FDynamicMeshInstance CreateDynamic( StringView path, RootID root = RootID::External );

	private:

		render::FRendererContext* mContext = nullptr;

		StaticMeshHandle mDefaultMesh; // Fallback mesh used when no mesh is assigned.
		StaticMeshHandle mErrorMesh;   // Fallback mesh used when loading fails.

		std::unordered_map<uint64, StaticMeshHandle> mStaticMeshCache; // Path hash -> handle cache.
		cstd::HandlePool<StaticMeshHandle, FStaticMeshResource> mStaticMeshes{ limits::gMaxModels };

		std::vector<FVertex> mBasicVertices = {
			// position                normal               uv
			{{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
			{{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
			{{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
			{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
		};

		std::vector<uint32> mBasicIndices = { 0, 1, 2, 2, 3, 0 };

		/* @brief Initializes built-in default and error meshes. */
		void init( );

		/* @brief Uploads mesh data to the GPU and returns render resources.
		* @param type   Mesh type (Static or Dynamic).
		* @param data   CPU-side mesh data to upload.
		* @return Render resources containing VBO, EBO, and VAO handles.
		*/
		detail::FRenderResources upload_gpu( detail::MeshType type, const FMeshData& data );

		/* @brief Creates and uploads built-in meshes (default, error). */
		void create_meshes( );

	};

} // namespace lum