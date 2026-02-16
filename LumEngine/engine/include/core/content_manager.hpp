#pragma once

#include "core/core_common.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_device.hpp"

namespace lum {

	// TODO automatically creates and stores rhi handles to textures, models, sounds and materials
	class ContentManager {
	public:

		ContentManager(rhi::RenderDevice* device) : mRenderDevice(device) {}

		void RegisterTexture(ccharptr name, ccharptr path) {

		}


	private:

		rhi::RenderDevice* mRenderDevice = nullptr;

		std::unordered_map<uint64, uint64> mTexturePaths;

		std::unordered_map<uint64, rhi::TextureHandle> mTextureCache;

	};

}