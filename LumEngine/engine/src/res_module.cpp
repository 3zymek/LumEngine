//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//

#include "res_module.hpp"
#include "platform_module.hpp"

namespace lum {

	void MResourceModule::Initialize( MPlatformModule& platform ) {

		mTextureMgr.Initialize( platform.mRenderDevice );
		mMeshMgr.Initialize( platform.mRenderDevice );
		mShaderMgr.Initialize( platform.mRenderDevice );
		mMaterialMgr.Initialize( platform.mRenderDevice, &mTextureMgr );

	}

} // namespace lum