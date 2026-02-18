#pragma once
#include "core/utils/handle_pool.hpp"
#include "core/utils/asset_loader.hpp"
#include "core/core_common.hpp"
#include "rhi/core/rhi_device.hpp"
namespace lum {

	class MShaderManager {
	public:

		MShaderManager ( rhi::RDevice* device ) : mRenderDevice(device) {}

		rhi::RShaderHandle LoadShader (ccharptr vertexPath, ccharptr fragmentPath, ERootID root = ERootID::External) {
		
			uint64 hash = HashStr(vertexPath) / HashStr(fragmentPath);

			if (mShaders.contains(hash))
				return mShaders[hash];

			std::optional<String> vertexData = AssetLoader::LoadShader(root, vertexPath);

			if (!vertexData) {
				LUM_LOG_FATAL("Failed to load shader %s: %s", vertexPath, AssetLoader::GetErrorMessage());
				LUM_DEBUGBREAK();
			}

			std::optional<String> fragmentData = AssetLoader::LoadShader(root, fragmentPath);
			
			if (!fragmentData) {
				LUM_LOG_FATAL("Failed to load shader %s: %s", vertexPath, AssetLoader::GetErrorMessage());
				LUM_DEBUGBREAK();
			}
			
			rhi::RShaderDescriptor desc;
			desc.mVertexContent = vertexData.value().c_str();
			desc.mFragmentContent = fragmentData.value().c_str();
			rhi::RShaderHandle shader = mRenderDevice->CreateShader(desc);
		
			mShaders[hash] = shader;

			return shader;
		}

	private:

		rhi::RDevice* mRenderDevice = nullptr;

		std::unordered_map<uint64, rhi::RShaderHandle> mShaders;
	

	};

}