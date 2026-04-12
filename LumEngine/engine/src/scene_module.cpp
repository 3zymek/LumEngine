//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//

#include "scene_module.hpp"
#include "render_module.hpp"
#include "res_module.hpp"
#include "audio_module.hpp"
#include "event/event_bus.hpp"

namespace lum {

	void MSceneModule::Initialize( MResourceModule& res, MRenderModule& render, MAudioModule& audio, ev::EventBus& bus ) {

		FSceneManagerContext ctx;
		ctx.mAudioMgr = &audio.mAudioMgr;
		ctx.mMaterialMgr = &res.mMaterialMgr;
		ctx.mMeshMgr = &res.mMeshMgr;
		ctx.mTextureMgr = &res.mTextureMgr;
		ctx.mShaderMgr = &res.mShaderMgr;
		ctx.mRenderer = &render.mRenderer;
		ctx.mEventBus = &bus;

		mSceneMgr.Initialize( ctx );

	}

} // namespace lum