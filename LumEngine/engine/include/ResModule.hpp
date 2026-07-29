//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//
#pragma once

#include "Render/TextureManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/ShaderManager.hpp"
#include "Render/MaterialManager.hpp"

namespace lum {

	class PlatformModule;

	// Owns all GPU-side resource managers (textures, meshes, shaders, materials).
	// Must be initialized after PlatformModule, before RenderModule.
	class ResourceModule {
	public:

		TextureManager		mTextureMgr;
		MaterialManager	mMaterialMgr;
		MeshManager		mMeshMgr;
		ShaderManager		mShaderMgr;

		void Initialize( PlatformModule& platform,  ev::EventBus& bus );

	private:

		render::RendererContext mCtx;

	};

} // namespace lum