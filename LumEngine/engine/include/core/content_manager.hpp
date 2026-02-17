#pragma once

#include "core/core_common.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_device.hpp"

namespace lum {

	// TODO automatically creates and stores rhi handles to textures, models, sounds and materials
	class AssetManager {
	public:

		AssetManager(rhi::RDevice* device) : mRenderDevice(device) {}


	private:

		rhi::RDevice* mRenderDevice = nullptr;

	};

}