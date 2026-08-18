//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//

#include "RenderModule.hpp"
#include "PlatformModule.hpp"
#include "ResModule.hpp"
#include "Event/EventBus.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void RenderModule::Initialize( const RenderModuleCreateInfo& info ) {

		render::RendererContext ctx;
		ctx.mMaterialMgr = &info.mResourceModule->mMaterialMgr;
		ctx.mMeshMgr = &info.mResourceModule->mMeshMgr;
		ctx.mTextureMgr = &info.mResourceModule->mTextureMgr;
		ctx.mRenderDev = info.mRenderDev;
		ctx.mShaderMgr = &info.mResourceModule->mShaderMgr;
		ctx.mEvBus = info.mEventBus;
		mRenderer.Initialize( ctx, info.mViewportWidth, info.mViewportHeight );

		mRenderSys.Initialize( &mRenderer );

	}

} // namespace lum