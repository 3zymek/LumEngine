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

		mCtx.mEventBus			= bus;
		mCtx.mMeshMgr		= mMeshMgr;
		mCtx.mMaterialMgr	= mMaterialMgr;
		mCtx.mShaderMgr		= mShaderMgr;
		mCtx.mTextureMgr	= mTextureMgr;
		mCtx.mRenderDev		= platform.mRenderDevice.get( );
		
		mCtx.Validate( );

		mTextureMgr.Initialize( *platform.mRenderDevice.get() );
		mMeshMgr.Initialize( mCtx );
		mShaderMgr.Initialize( *platform.mRenderDevice.get() );
		mMaterialMgr.Initialize( mCtx );

	}

} // namespace lum