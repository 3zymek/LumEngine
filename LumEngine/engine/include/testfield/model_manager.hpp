#pragma once

#include "rhi/core/rhi_device.hpp"

#include "core/core_common.hpp"
#include "core/utils/asset_loader.hpp"
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
	}

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

		MMeshManager(rhi::RDevice* device) : mRenderDevice(device) { create_meshes(); }

		FStaticMeshResource GetStatic(StaticMeshHandle handle) {
			if (mStaticMeshes.Contains(handle))
				return mStaticMeshes[handle];
			else 
				return mStaticMeshes[mDefaultMesh];
		}

		StaticMeshHandle CreateStatic(ccharptr path, ERootID root = ERootID::External) {

			uint64 hash = HashStr(path);

			if (mStaticMeshCache.contains(hash))
				return mStaticMeshCache[hash];

			std::optional<FMeshData> data = AssetLoader::LoadMesh(root, path);

			if (!data) {
				LUM_LOG_ERROR("Failed to load model %s: %s", path, AssetLoader::GetErrorMessage());
				return mErrorMesh;
			}

			detail::FRenderResources res = upload_gpu(detail::EMeshType::Static, data.value());

			FStaticMeshResource meshResource;
			meshResource.mVbo = res.mVbo;
			meshResource.mEbo = res.mEbo;
			meshResource.mVao = res.mVao;
			meshResource.mNumIndices = data.value().mIndices.size();

			StaticMeshHandle meshHandle = mStaticMeshes.Append(std::move(meshResource));

			mStaticMeshCache[hash] = meshHandle;

			return meshHandle;
		}

		FDynamicMeshInstance CreateDynamic(ccharptr path, ERootID root = ERootID::External) {
			
			std::optional<FMeshData> data = AssetLoader::LoadMesh(root, path);

			if (!data) {
				LUM_LOG_ERROR("Failed to load model %s: %s", path, AssetLoader::GetErrorMessage());
				FMeshData fallback;
				fallback.mVertices = mBasicVertices;
				fallback.mIndices = mBasicIndices;
				data = fallback;
			}

			detail::FRenderResources res = upload_gpu(detail::EMeshType::Dynamic, data.value());

			FDynamicMeshInstance meshInstance;
			meshInstance.mData = data.value();
			meshInstance.mVbo = res.mVbo;
			meshInstance.mEbo = res.mEbo;
			meshInstance.mVao = res.mVao;

			return meshInstance;
		}

	private:

		rhi::RDevice* mRenderDevice = nullptr;

		StaticMeshHandle mDefaultMesh;
		StaticMeshHandle mErrorMesh;

		std::unordered_map<uint64, StaticMeshHandle> mStaticMeshCache;
		cstd::HandlePool<StaticMeshHandle, FStaticMeshResource> mStaticMeshes { limits::gMaxModels };

		std::vector<Vertex> mBasicVertices = {
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

		detail::FRenderResources upload_gpu(detail::EMeshType type, const FMeshData& data) {
			
			Flags<rhi::RMapFlag> mapFlag{};
			rhi::RBufferUsage usage{};
			
			if (type == detail::EMeshType::Static) {

				mapFlag = rhi::RMapFlag::Read;
				usage = rhi::RBufferUsage::Static;

			}
			else if (type == detail::EMeshType::Dynamic) {

				mapFlag = rhi::RMapFlag::Read | rhi::RMapFlag::Write;
				usage = rhi::RBufferUsage::Dynamic;

			}
			
			detail::FRenderResources res;
			
			rhi::RBufferDescriptor vboDesc;
			vboDesc.mBufferUsage = usage;
			vboDesc.mData = data.mVertices.data();
			vboDesc.mMapFlags = mapFlag;
			vboDesc.mSize = ByteSize(data.mVertices);
			vboDesc.mBufferType = rhi::RBufferType::Vertex;
			res.mVbo = mRenderDevice->CreateBuffer(vboDesc);
			
			rhi::RBufferDescriptor eboDesc;
			eboDesc.mBufferUsage = usage;
			eboDesc.mData = data.mIndices.data();
			eboDesc.mMapFlags = mapFlag;
			eboDesc.mSize = ByteSize(data.mIndices);
			eboDesc.mBufferType = rhi::RBufferType::Element;
			res.mEbo = mRenderDevice->CreateBuffer(eboDesc);
			
			rhi::RVertexAttribute vaoAttrib[3];
			
			auto& position = vaoAttrib[0];
			position.mFormat = rhi::RDataFormat::Vec3;
			position.mRelativeOffset = offsetof(Vertex, position);
			position.mShaderLocation = LUM_LAYOUT_POSITION;
			
			auto& normal = vaoAttrib[1];
			normal.mFormat = rhi::RDataFormat::Vec3;
			normal.mRelativeOffset = offsetof(Vertex, normal);
			normal.mShaderLocation = LUM_LAYOUT_NORMAL;
			
			auto& uv = vaoAttrib[2];
			uv.mFormat = rhi::RDataFormat::Vec2;
			uv.mRelativeOffset = offsetof(Vertex, uv);
			uv.mShaderLocation = LUM_LAYOUT_UV;
			
			rhi::RVertexLayoutDescriptor vaoDesc;
			vaoDesc.mAttributes = vaoAttrib;
			vaoDesc.mStride = sizeof(Vertex);
			res.mVao = mRenderDevice->CreateVertexLayout(vaoDesc, res.mVbo);

			mRenderDevice->AttachElementBufferToLayout(res.mEbo, res.mVao);
			
			return res;
		}

		void create_meshes() {
			{ // Default mesh
				
				FMeshData data;
				data.mVertices = mBasicVertices;
				data.mIndices = mBasicIndices;
				
				detail::FRenderResources res = upload_gpu(detail::EMeshType::Static, data);

				FStaticMeshResource staticMesh;
				staticMesh.mVbo = res.mVbo;
				staticMesh.mEbo = res.mEbo;
				staticMesh.mVao = res.mVao;
				staticMesh.mNumIndices = data.mIndices.size();
				
				mDefaultMesh = mStaticMeshes.Append(std::move(staticMesh));
				
			}
			{ // Error mesh
				std::optional<FMeshData> data = AssetLoader::LoadMesh(ERootID::Internal, "models/ERRORText.fbx");
				if (!data) {
					LUM_LOG_ERROR("Failed to load fallback error model: %s", AssetLoader::GetErrorMessage());
					mErrorMesh = mDefaultMesh;
					return;
				}
				detail::FRenderResources res = upload_gpu(detail::EMeshType::Static, data.value());

				FStaticMeshResource staticMesh;
				staticMesh.mVbo = res.mVbo;
				staticMesh.mEbo = res.mEbo;
				staticMesh.mVao = res.mVao;
				staticMesh.mNumIndices = data.value().mIndices.size();

				mErrorMesh = mStaticMeshes.Append(std::move(staticMesh));

			}

		}

	};


}