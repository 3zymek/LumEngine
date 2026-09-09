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

//=======================================================//
	// Public
//=======================================================//

	void RenderModule::Initialize( const RenderModuleCreateInfo& info ) {

		render::RendererContext ctx{};
		ctx.m_MaterialMgr	= info.m_ResourceModule( ).m_MaterialMgr;
		ctx.m_MeshMgr		= info.m_ResourceModule( ).m_MeshMgr;
		ctx.m_TextureMgr		= info.m_ResourceModule( ).m_TextureMgr;
		ctx.m_ShaderMgr		= info.m_ResourceModule( ).m_ShaderMgr;
		ctx.m_RenderDev		= info.m_RenderDev;
		ctx.m_EventBus		= info.m_EventBus;

		m_Renderer.Initialize( ctx, info.m_ViewportWidth, info.m_ViewportHeight );
		m_RenderSys.Initialize( &m_Renderer );

	}

} // namespace lum