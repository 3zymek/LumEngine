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
		ctx.m_AudioMgr = info.m_AudioModule( ).m_AudioMgr;
		ctx.m_MaterialMgr = info.m_ResourceModule( ).m_MaterialMgr;
		ctx.m_MeshMgr = info.m_ResourceModule( ).m_MeshMgr;
		ctx.m_TextureMgr = info.m_ResourceModule( ).m_TextureMgr;
		ctx.m_ShaderMgr = info.m_ResourceModule( ).m_ShaderMgr;
		ctx.m_Renderer = info.m_RenderModule( ).m_Renderer;
		ctx.m_EventBus = info.m_EventBus;

		m_SceneMgr.Initialize( ctx );

	}

} // namespace lum