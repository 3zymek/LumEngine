//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//

#include "render_module.hpp"
#include "platform_module.hpp"
#include "res_module.hpp"
#include "event/event_bus.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MRenderModule::Initialize( MPlatformModule& platform, MResourceModule& res, ev::EventBus& bus ) {

		render::FRendererContext ctx;
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