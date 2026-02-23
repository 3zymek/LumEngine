#pragma once
#include "rhi/core/rhi_device.hpp"

namespace lum {

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
}