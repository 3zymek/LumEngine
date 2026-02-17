#pragma once

#include "core/core_common.hpp"
#include "testfield/texture_manager.hpp"

namespace lum {

	class SRenderer {
	public:
		
		SRenderer() {}
		SRenderer(rhi::RDevice* device, CTextureManager* textureMgr) : mRenderDevice(device), mTextureManager(textureMgr) {}


		CTextureManager* mTextureManager = nullptr;

	private:

		rhi::RDevice* mRenderDevice = nullptr;

	};

}