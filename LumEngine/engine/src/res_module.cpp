//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//

#include "res_module.hpp"
#include "platform_module.hpp"

namespace lum {

	void MResourceModule::Initialize( MPlatformModule& platform, ev::EventBus& bus ) {

		mCtx.mEvBus = &bus;
		mCtx.mMaterialMgr = &mMaterialMgr;
		mCtx.mMeshMgr = &mMeshMgr;
		mCtx.mRenderDev = platform.mRenderDevice;
		mCtx.mShaderMgr = &mShaderMgr;
		mCtx.mTextureMgr = &mTextureMgr;
		
		render::ValidateRendererContext( mCtx );

		mTextureMgr.Initialize( platform.mRenderDevice );
		mMeshMgr.Initialize( &mCtx );
		mShaderMgr.Initialize( platform.mRenderDevice );
		mMaterialMgr.Initialize( &mCtx );

	}

} // namespace lum