//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//

#pragma once

#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"

namespace lum {

	struct MPlatformModule;

	// Owns all GPU-side resource managers (textures, meshes, shaders, materials).
	// Must be initialized after MPlatformModule, before MRenderModule.
	struct MResourceModule {

		MTextureManager		mTextureMgr;
		MMaterialManager	mMaterialMgr;
		MMeshManager		mMeshMgr;
		MShaderManager		mShaderMgr;

		void Initialize( MPlatformModule& platform );

	};

} // namespace lum