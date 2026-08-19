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

	void SceneModule::Initialize( const SceneModuleCreateInfo& info ) {

		SceneManagerContext ctx;
		ctx.mAudioMgr = info.mAudioModule( ).mAudioMgr;
		ctx.mMaterialMgr = info.mResourceModule( ).mMaterialMgr;
		ctx.mMeshMgr = info.mResourceModule( ).mMeshMgr;
		ctx.mTextureMgr = info.mResourceModule( ).mTextureMgr;
		ctx.mShaderMgr = info.mResourceModule( ).mShaderMgr;
		ctx.mRenderer = info.mRenderModule( ).mRenderer;
		ctx.mEventBus = info.mEventBus;

		mSceneMgr.Initialize( ctx );

	}

} // namespace lum