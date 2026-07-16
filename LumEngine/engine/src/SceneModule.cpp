//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//

#include "SceneModule.hpp"
#include "RenderModule.hpp"
#include "ResModule.hpp"
#include "AudioModule.hpp"
#include "Event/EventBus.hpp"

namespace lum {

	void MSceneModule::Initialize( MResourceModule& res, MRenderModule& render, MAudioModule& audio, ev::EventBus& bus ) {

		SceneManagerContext ctx;
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