//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Loads and manages static mesh resources using RHI.
//
//=============================================================================//

#include "render/mesh_manager.hpp"
#include "core/utils/asset_loader.hpp"
#include "render/mesh.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MMeshManager::Initialize( rhi::RenderDevice* device ) {

		mRenderDevice = device;
		init();

	}

	const FStaticMeshResource& MMeshManager::GetStatic( StaticMeshHandle handle ) {
		if (mStaticMeshes.Contains(handle))
			return mStaticMeshes[handle];
		else
			return mStaticMeshes[mDefaultMesh];
	}

	StaticMeshHandle MMeshManager::CreateStatic( ccharptr path, RootID root ) {

		uint64 hash = HashStr(path);

		if (mStaticMeshCache.contains(hash))
			return mStaticMeshCache[hash];

		std::optional<FMeshData> data = AssetLoader::LoadMesh(root, path);

		if (!data) {
			LUM_LOG_ERROR("Failed to load model %s: %s", path, AssetLoader::GetErrorMessage());
			return mErrorMesh;
		}

		detail::FRenderResources res = upload_gpu(detail::MeshType::Static, data.value());

		FStaticMeshResource meshResource;
		meshResource.mVbo = res.mVbo;
		meshResource.mEbo = res.mEbo;
		meshResource.mVao = res.mVao;
		meshResource.mNumIndices = data.value().mIndices.size();

		StaticMeshHandle meshHandle = mStaticMeshes.Append(std::move(meshResource));

		mStaticMeshCache[hash] = meshHandle;

		return meshHandle;
	}

	FDynamicMeshInstance MMeshManager::CreateDynamic(ccharptr path, RootID root ) {

		std::optional<FMeshData> data = AssetLoader::LoadMesh(root, path);

		if (!data) {
			LUM_LOG_ERROR("Failed to load model %s: %s", path, AssetLoader::GetErrorMessage());
			FMeshData fallback;
			fallback.mVertices = mBasicVertices;
			fallback.mIndices = mBasicIndices;
			data = fallback;
		}

		detail::FRenderResources res = upload_gpu(detail::MeshType::Dynamic, data.value());

		FDynamicMeshInstance meshInstance;
		meshInstance.mData = data.value();
		meshInstance.mVbo = res.mVbo;
		meshInstance.mEbo = res.mEbo;
		meshInstance.mVao = res.mVao;

		return meshInstance;
	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MMeshManager::init( ) {

		create_meshes();

	}

	detail::FRenderResources MMeshManager::upload_gpu( detail::MeshType type, const FMeshData& data ) {

		Flags<rhi::MapFlag> mapFlag{};
		rhi::BufferUsage usage{};

		if (type == detail::MeshType::Static) {

			mapFlag = rhi::MapFlag::None;
			usage = rhi::BufferUsage::Static;

		}
		else if (type == detail::MeshType::Dynamic) {

			mapFlag = rhi::MapFlag::Read | rhi::MapFlag::Write;
			usage = rhi::BufferUsage::Dynamic;

		}

		detail::FRenderResources res;

		rhi::FBufferDescriptor vboDesc;
		vboDesc.mBufferUsage = usage;
		vboDesc.mData = data.mVertices.data();
		vboDesc.mMapFlags = mapFlag;
		vboDesc.mSize = ByteSize(data.mVertices);
		vboDesc.mBufferType = rhi::BufferType::Vertex;
		res.mVbo = mRenderDevice->CreateBuffer(vboDesc);

		rhi::FBufferDescriptor eboDesc;
		eboDesc.mBufferUsage = usage;
		eboDesc.mData = data.mIndices.data();
		eboDesc.mMapFlags = mapFlag;
		eboDesc.mSize = ByteSize(data.mIndices);
		eboDesc.mBufferType = rhi::BufferType::Element;
		res.mEbo = mRenderDevice->CreateBuffer(eboDesc);

		rhi::FVertexAttribute vaoAttrib[5];

		auto& position = vaoAttrib[0];
		position.mFormat = rhi::DataFormat::Vec3;
		position.mRelativeOffset = offsetof(FVertex, mPosition);
		position.mShaderLocation = LUM_LAYOUT_POSITION;

		auto& normal = vaoAttrib[1];
		normal.mFormat = rhi::DataFormat::Vec3;
		normal.mRelativeOffset = offsetof(FVertex, mNormal);
		normal.mShaderLocation = LUM_LAYOUT_NORMAL;

		auto& uv = vaoAttrib[2];
		uv.mFormat = rhi::DataFormat::Vec2;
		uv.mRelativeOffset = offsetof(FVertex, mUv);
		uv.mShaderLocation = LUM_LAYOUT_UV;

		auto& tg = vaoAttrib[3];
		tg.mFormat = rhi::DataFormat::Vec3;
		tg.mRelativeOffset = offsetof(FVertex, mTangent);
		tg.mShaderLocation = LUM_LAYOUT_TANGENT;

		auto& btg = vaoAttrib[4];
		btg.mFormat = rhi::DataFormat::Vec3;
		btg.mRelativeOffset = offsetof(FVertex, mBitangent);
		btg.mShaderLocation = LUM_LAYOUT_BITANGENT;

		rhi::FVertexLayoutDescriptor vaoDesc;
		vaoDesc.mAttributes = vaoAttrib;
		vaoDesc.mStride = sizeof(FVertex);
		res.mVao = mRenderDevice->CreateVertexLayout(vaoDesc, res.mVbo);

		mRenderDevice->AttachElementBufferToLayout(res.mEbo, res.mVao);

		return res;
	}

	void MMeshManager::create_meshes( ) {
		{ // Default mesh

			FMeshData data;
			data.mVertices = mBasicVertices;
			data.mIndices = mBasicIndices;

			detail::FRenderResources res = upload_gpu(detail::MeshType::Static, data);

			FStaticMeshResource staticMesh;
			staticMesh.mVbo = res.mVbo;
			staticMesh.mEbo = res.mEbo;
			staticMesh.mVao = res.mVao;
			staticMesh.mNumIndices = data.mIndices.size();

			mDefaultMesh = mStaticMeshes.Append(std::move(staticMesh));

		}
		{ // Error mesh
			std::optional<FMeshData> data = AssetLoader::LoadMesh(RootID::Internal, "models/ERRORText.fbx");
			if (!data) {
				LUM_LOG_ERROR("Failed to load fallback error model: %s", AssetLoader::GetErrorMessage());
				mErrorMesh = mDefaultMesh;
				return;
			}
			detail::FRenderResources res = upload_gpu(detail::MeshType::Static, data.value());

			FStaticMeshResource staticMesh;
			staticMesh.mVbo = res.mVbo;
			staticMesh.mEbo = res.mEbo;
			staticMesh.mVao = res.mVao;
			staticMesh.mNumIndices = data.value().mIndices.size();

			mErrorMesh = mStaticMeshes.Append(std::move(staticMesh));

		}

	}

} // namespace lum