//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shader resource management.
//          Provides loading and caching of vertex and fragment shaders
//          to avoid redundant GPU resource creation.
//
//=============================================================================//
#pragma once

#include "Core/Utils/ResourceLoader.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Render/ShaderManager.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void ShaderManager::Initialize( rhi::IRenderDevice& device ) {
		mRenderDevice = &device;
	}

	rhi::ShaderHandle ShaderManager::LoadShader( Path vertexPath, Path fragmentPath, ResourceRoot root ) {

		uint64 hash = vertexPath.Hash( ) ^ fragmentPath.Hash( );
		if (mShaders.contains( hash ))
			return mShaders[ hash ];

		auto vertexData = ResourceLoader::BuildShaderSource( root, vertexPath );
		if (!vertexData) {
			LUM_LOG_ERROR( "Failed to load shader %s: %s", vertexPath.ToString( ), vertexData.GetError( ) );
			return {};
		}

		auto fragmentData = ResourceLoader::BuildShaderSource( root, fragmentPath );
		if (!fragmentData) {
			LUM_LOG_ERROR( "Failed to load shader %s: %s", fragmentPath.ToString( ), fragmentData.GetError( ) );
			return {};
		}
		
		rhi::ShaderCreateInfo desc{};
		desc.mVertexContent = vertexData.ValueRef( );
		desc.mFragmentContent = fragmentData.ValueRef( );
		
		rhi::ShaderHandle shader = mRenderDevice->CreateShader( desc );
		
		mShaders[ hash ] = shader;

		return shader;

	}

} // namespace lum