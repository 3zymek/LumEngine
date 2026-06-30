//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shader resource management.
//          Provides loading and caching of vertex and fragment shaders
//          to avoid redundant GPU resource creation.
//
//=============================================================================//
#pragma once

#include "core/utils/resource_loader.hpp"
#include "rhi/core/rhi_device.hpp"
#include "render/shader_manager.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MShaderManager::Initialize( rhi::RenderDevice* device ) {
		mRenderDevice = device;
	}

	rhi::ShaderHandle MShaderManager::LoadShader( ccharptr vertexPath, ccharptr fragmentPath, ResourceRoot root ) {

		uint64 hash = HashString(vertexPath) ^ HashString(fragmentPath);
		if (mShaders.contains(hash))
			return mShaders[hash];

		std::optional<String> vertexData = ResourceLoader::BuildShaderSource(root, vertexPath);
		if (!vertexData.has_value()) {
			LUM_LOG_ERROR("Failed to load shader %s: %s", vertexPath, ResourceLoader::GetErrorMessage());
			return {};
		}

		std::optional<String> fragmentData = ResourceLoader::BuildShaderSource(root, fragmentPath);
		if (!fragmentData.has_value()) {
			LUM_LOG_ERROR("Failed to load shader %s: %s", vertexPath, ResourceLoader::GetErrorMessage());
			return {};
		}

		rhi::ShaderCreateInfo desc{};
		desc.mVertexContent = vertexData.value();
		desc.mFragmentContent = fragmentData.value();

		rhi::ShaderHandle shader = mRenderDevice->CreateShader(desc);

		mShaders[hash] = shader;

		return shader;

	}

} // namespace lum