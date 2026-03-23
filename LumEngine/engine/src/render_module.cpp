//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//

#include "render_module.hpp"
#include "platform_module.hpp"
#include "res_module.hpp"
#include "event/event_bus.hpp"

namespace lum {

	void MRenderModule::Initialize( MPlatformModule& platform, MResourceModule& res, ev::EventBus& bus ) {

		render::FRendererContext ctx;
		ctx.mMaterialMgr = &res.mMaterialMgr;
		ctx.mMeshMgr = &res.mMeshMgr;
		ctx.mTextureMgr = &res.mTextureMgr;
		ctx.mRenderDevice = platform.mRenderDevice;
		ctx.mShaderMgr = &res.mShaderMgr;
		ctx.mEventBus = &bus;
<<<<<<< HEAD
		ctx.mWindow = &platform.mWindow;
=======
>>>>>>> f8ece7c (Editor in works)
		mRenderer.Initialize( ctx );

		mRenderSys.Initialize( &mRenderer );

	}

} // namespace lum