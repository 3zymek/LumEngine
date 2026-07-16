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

	void MRenderModule::Initialize( MPlatformModule& platform, MResourceModule& res, ev::EventBus& bus ) {

		render::RendererContext ctx;
		ctx.mMaterialMgr = &res.mMaterialMgr;
		ctx.mMeshMgr = &res.mMeshMgr;
		ctx.mTextureMgr = &res.mTextureMgr;
		ctx.mRenderDev = platform.mRenderDevice;
		ctx.mShaderMgr = &res.mShaderMgr;
		ctx.mEvBus = &bus;
		mRenderer.Initialize( ctx );

		mRenderSys.Initialize( &mRenderer );

	}

} // namespace lum