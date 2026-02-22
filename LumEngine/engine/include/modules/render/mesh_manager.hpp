#pragma once

#include "rhi/core/rhi_device.hpp"

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"

namespace lum {

	namespace detail {

		enum class EMeshType : byte {
			Static,
			Dynamic
		};

		struct FRenderResources {

			rhi::RBufferHandle mVbo;
			rhi::RBufferHandle mEbo;
			rhi::RVertexLayoutHandle mVao;

		};

	} // namespace lum::detail

	struct StaticMeshHandle : cstd::BaseHandle<uint32> {};
	struct DynamicMeshHandle : cstd::BaseHandle<uint32> {};

	struct FStaticMeshResource {

		rhi::RBufferHandle mVbo;
		rhi::RBufferHandle mEbo;
		rhi::RVertexLayoutHandle mVao;
		uint32 mNumIndices = 0;

	};

	struct FDynamicMeshInstance {

		FMeshData mData;
		rhi::RBufferHandle mVbo;
		rhi::RBufferHandle mEbo;
		rhi::RVertexLayoutHandle mVao;
		bool bDirty = false;

	};

	class MMeshManager {
	public:

		MMeshManager( ) {}

		void Initialize( rhi::RDevice* device );

		FStaticMeshResource GetStatic( StaticMeshHandle handle );

		StaticMeshHandle CreateStatic( ccharptr path, ERootID root = ERootID::External );

		FDynamicMeshInstance CreateDynamic( ccharptr path, ERootID root = ERootID::External );

	private:

		rhi::RDevice* mRenderDevice = nullptr;

		StaticMeshHandle mDefaultMesh;
		StaticMeshHandle mErrorMesh;

		std::unordered_map<uint64, StaticMeshHandle> mStaticMeshCache;
		cstd::HandlePool<StaticMeshHandle, FStaticMeshResource> mStaticMeshes { limits::gMaxModels };

		std::vector<rhi::FVertex> mBasicVertices = {
			// position              // normal            // uv
			{{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
			{{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
			{{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
			{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
		};

		std::vector<uint32> mBasicIndices = {
			0, 1, 2,
			2, 3, 0
		};


		/* Init internal implementation */
		void init();

		detail::FRenderResources upload_gpu( detail::EMeshType type, const FMeshData& data );

		void create_meshes( );

	};


} // namespace lum