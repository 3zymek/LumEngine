//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Loads and manages static mesh resources using RHI.
//
//=============================================================================//
#pragma once
#include "Render/RenderCommon.hpp"
#include "Render/Mesh.hpp"

namespace lum {

	namespace detail {

		enum class LUM_API MeshType : byte {
			Static,
			Dynamic
		};

		/* @brief Stores raw GPU buffer handles for a mesh (VBO, EBO, VAO). */
		struct FRenderResources {
			rhi::BufferHandle			mVbo;
			rhi::BufferHandle			mEbo;
			rhi::VertexLayoutHandle	mVao;
		};

	} // namespace lum::detail

	/* @brief Loads and manages static mesh resources on the GPU.
	*
	* Handles mesh loading via Assimp, GPU upload, and handle-based access.
	* Meshes are cached by path hash to avoid duplicate uploads.
	* Provides built-in default and error meshes as fallbacks.
	*/
	class LUM_API MMeshManager {
	public:

		MMeshManager() {}

		void Initialize( render::RendererContext* ctx );

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
		StaticMeshHandle CreateStatic( StringView path, ResourceRoot root = ResourceRoot::External );

		/* @brief Loads a dynamic mesh instance from disk.
		* @param path Path to the mesh file.
		* @param root Root directory identifier.
		* @return Dynamic mesh instance.
		*/
		FDynamicMeshInstance CreateDynamic( StringView path, ResourceRoot root = ResourceRoot::External );

	private:

		render::RendererContext* mContext = nullptr;

		StaticMeshHandle mDefaultMesh; // Fallback mesh used when no mesh is assigned.
		StaticMeshHandle mErrorMesh;   // Fallback mesh used when loading fails.

		std::unordered_map<uint64, StaticMeshHandle> mStaticMeshCache; // Path hash -> handle cache.
		cstd::HandlePool<StaticMeshHandle, FStaticMeshResource> mStaticMeshes{ limits::kMaxModels };

		std::vector<Vertex> mDefaultVertices = {
			// position                normal               uv
			{{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
			{{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
			{{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
			{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
		};

		std::vector<uint32> mDefaultIndices = { 0, 1, 2, 2, 3, 0 };

		/* @brief Initializes built-in default and error meshes. */
		void init( );

		/* @brief Uploads mesh data to the GPU and returns render resources.
		* @param type   Mesh type (Static or Dynamic).
		* @param data   CPU-side mesh data to upload.
		* @return Render resources containing VBO, EBO, and VAO handles.
		*/
		detail::FRenderResources upload_gpu( detail::MeshType type, const MeshGeometry& data );

		/* @brief Creates and uploads built-in meshes (default, error). */
		void create_meshes( );

	};

} // namespace lum