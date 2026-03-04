//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shader resource management.
//          Provides loading and caching of vertex and fragment shaders
//          to avoid redundant GPU resource creation.
//
//=============================================================================//
#pragma once

#include "core/utils/asset_loader.hpp"
#include "rhi/core/rhi_device.hpp"
#include "render/shader_manager.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MShaderManager::Initialize( rhi::RDevice* device ) {
		mRenderDevice = device;
	}

	rhi::RShaderHandle MShaderManager::LoadShader( ccharptr vertexPath, ccharptr fragmentPath, RootID root ) {

		uint64 hash = HashStr(vertexPath) ^ HashStr(fragmentPath);
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

} // namespace lum