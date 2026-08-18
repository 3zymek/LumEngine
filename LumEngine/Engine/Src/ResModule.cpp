//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//

#include "ResModule.hpp"
#include "PlatformModule.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void ResourceModule::Initialize( PlatformModule& platform, ev::EventBus& bus ) {

		mCtx.mEvBus = &bus;
		mCtx.mMaterialMgr = &mMaterialMgr;
		mCtx.mMeshMgr = &mMeshMgr;
		mCtx.mRenderDev = platform.mRenderDevice.get();
		mCtx.mShaderMgr = &mShaderMgr;
		mCtx.mTextureMgr = &mTextureMgr;
		
		mCtx.Validate( );

		mTextureMgr.Initialize( *platform.mRenderDevice.get() );
		mMeshMgr.Initialize( mCtx );
		mShaderMgr.Initialize( *platform.mRenderDevice.get() );
		mMaterialMgr.Initialize( mCtx );

	}

} // namespace lum